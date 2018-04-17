from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # query SQL database for user info
    funds = db.execute("SELECT cash, username FROM users WHERE id = :id GROUP BY username", id = session["user_id"])
    stocks = db.execute("SELECT stock, symbol, SUM(shares) AS quantities, SUM(price) AS total FROM portfolio WHERE user = :user GROUP BY stock, symbol ORDER BY stock", user = funds[0]["username"])
    totalval = 0

    # get price for each group (ie AAPL) with help from lookup function (which remember, returns a dict)
    stockprices = []
    for stock in stocks:
        symbol = str(stock["symbol"])
        quote = lookup(symbol)
        stockprices.append(quote['price'])
        totalval += stock["quantities"] * quote['price']

    return render_template("index.html", stocks = stocks, funds = funds, stockprices = stockprices, totalval = totalval)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via GET
    if request.method == "GET":
        return render_template("buy.html")

    else:
        # ensure a valid symbol was submitted
        if lookup(request.form.get("symbol")) is None:
            return apology("must provide valid stock symbol")

        # ensure a valid number was submitted
        elif not request.form.get("tobuy") or request.form.get("tobuy") < '1':
            return apology("must enter a valid number")

        else:
            # create objects for the variables to be used and stored
            shares = int(request.form.get("tobuy"))
            stock = lookup(request.form.get("symbol"))
            user = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])[0]["username"]
            name = stock['name']
            symbol = stock['symbol']
            price = stock['price']
            cost = shares * price
            balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"]

            # update balance, portfolio, and history with purchase if purchase is valid
            if balance > cost:
                balance -= cost
                db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = balance, id = session["user_id"])
                db.execute("INSERT INTO portfolio (user, symbol, shares, price, stock) VALUES (:user, :symbol, :shares, :price, :stock)", user = user, symbol = symbol, shares = shares, price = cost, stock = name)
                db.execute("INSERT INTO activity (user, symbol, shares, price, stock, type) VALUES (:user, :symbol, :shares, :price, :stock, :type)", user = user, symbol = symbol, shares = shares, price = cost, stock = name, type = "purchase")
                return redirect(url_for("index"))
            else:
                return apology("insufficient funds")

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Allow users to view and add to their cash balance."""
    funds = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])

    if request.method == "GET":
        return render_template("cash.html", funds = funds)
    else:
        if int(request.form.get("amount")) > 0:
            nucash = funds[0]['cash'] + int(request.form.get("amount"))
            db.execute("UPDATE users SET cash = :nucash WHERE id = :id", nucash = nucash, id = session["user_id"])
            return redirect(url_for("cash"))
        else:
            return apology("must enter a valid number")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    funds = db.execute("SELECT username FROM users WHERE id = :id GROUP BY username", id = session["user_id"])
    transactions = db.execute("SELECT * FROM activity WHERE user = :user", user = funds[0]["username"])
    return render_template("history.html", transactions = transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    else:
        # ensure a valid symbol was submitted
        if lookup(request.form.get("symbol")) is None:
            return apology("must provide valid stock symbol")

        # display stock information
        else:
            return render_template("quoted.html", stock = lookup(request.form.get("symbol")))

@app.route("/register", methods=["GET", "POST"])
def register():

    """Register user."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password and confirmation match
        if request.form.get("passwordconf") != request.form.get("password"):
            return apology("password confirmation must match")

        # hash password
        hash = pwd_context.encrypt(request.form.get("password"))

        # add user to database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username = request.form.get("username"), hash = hash)
        if not result:
            return apology("username already exists")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))

        # log user in
        session["user_id"] = rows[0]["id"]
        return redirect(url_for("index"))

    # display registration page
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # if user reached route via GET
    if request.method == "GET":
        funds = db.execute("SELECT cash, username FROM users WHERE id = :id GROUP BY username", id = session["user_id"])
        stocks = db.execute("SELECT stock, symbol, SUM(shares) AS quantities, SUM(price) AS total FROM portfolio WHERE user = :user GROUP BY stock, symbol ORDER BY stock", user = funds[0]["username"])
        return render_template("sell.html", stocks = stocks, funds = funds)

    else:
        # ensure a valid symbol was submitted
        if request.form.get("symbol") is None:
            return apology("must provide valid stock symbol")

        # ensure a valid number was submitted
        elif not request.form.get("tosell") or request.form.get("tosell") < '1':
            return apology("must enter a valid number")

        else:
            stock = lookup(request.form.get("symbol"))
            stocks = db.execute("SELECT SUM(shares) AS quantities FROM portfolio WHERE symbol = :symbol", symbol = stock['symbol'])
            user = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])[0]["username"]

            # create objects for the variables to be used and stored
            saleamt = int(request.form.get("tosell"))
            symbol = stock['symbol']
            currentprice = stock['price']
            saleprice = saleamt * currentprice
            balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"]

            # update balance and portfolio with sale if sale is valid
            if stocks[0]["quantities"] >= saleamt:
                stocks[0]["quantities"] -= saleamt
                balance += saleprice
                db.execute("UPDATE users SET cash = :balance WHERE id = :id", balance = balance, id = session["user_id"])
                if stocks[0]["quantities"] == 0:
                    db.execute("DELETE FROM portfolio WHERE symbol = :symbol", symbol = stock['symbol'])
                    db.execute("INSERT INTO activity (user, symbol, shares, price, stock, type) VALUES (:user, :symbol, :shares, :price, :stock, :type)", user = user, symbol = symbol, shares = saleamt, price = saleprice, stock = stock['name'], type = "sale")
                else:
                    db.execute("UPDATE portfolio SET shares = :shares WHERE symbol = :symbol", shares = stocks[0]["quantities"], symbol = stock['symbol'])
                    db.execute("INSERT INTO activity (user, symbol, shares, price, stock, type) VALUES (:user, :symbol, :shares, :price, :stock, :type)", user = user, symbol = symbol, shares = saleamt, price = saleprice, stock = stock['name'], type = "sale")
                return redirect(url_for("index"))
            else:
                return apology("must enter a valid number")
