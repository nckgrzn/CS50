import nltk
import sys

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # create sets to store text files
        self.positives = set()
        self.negatives = set()

        # store positive words into positive set
        skips = (" ", ";")
        with open("positive-words.txt") as pfile:
            for line in pfile:
                if line.startswith(skips) != True:
                    self.positives.add(line.strip())

        # store negatives words into negative set
        with open("negative-words.txt") as nfile:
            for line in nfile:
                if line.startswith(skips) != True:
                    self.negatives.add(line.strip())


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # tokenize the user entry
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        # create variable to keep score with
        score = 0

        # determine if the user input is found in the sets created
        for i in range(len(tokens)):
            if str.lower(tokens[i]) in self.positives:
                score += 1
            elif str.lower(tokens[i]) in self.negatives:
                score -= 1
        return score
