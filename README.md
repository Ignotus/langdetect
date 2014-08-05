# Shared task for AACIMP ACS 2014

## Text language detection

Given a text return its language or a map of languages with confidence.

Example:

    Text:
    This is a test.

    Output:
    'en': 0.95, 'af': 0.03

Existing tools:

- [language-detection](http://code.google.com/p/language-detection/) library
  based on character LM

## Task

Create a software tool, that can be run from the Unix command-line and given a text
print to standard output the map of detected languages with confidence scores
in JSON format.

Example usage:

    $ detect-lang file.txt    <-- console invocation
    {"en": 0.67, "bg" 0.3}    <-- standard output

The text can be in one of the following languages:
Afrikaans (af), English (en), German (de), Russian (ru), Ukrainian (ua), Polish (pl), Bulgarian (bg).


## Submission guidelines

A test data set of documents extracted randomly from Wikipedia will be published.
Evaluation will be performed on a similarly distributed held-out data set form the same source.
Sizes of documents will vary from 3 to 3000 words.

Evaluation formula (in Python):

    score = sum(map(lambda x,y: x[y] or 0, detected_langs, correct_langs))

Where `detected_langs` are maps produced by the tool,
      `correct_langs` is a list of languages for the texts given.

Submission should be performed in the form of a pull request to this repository.

1. You should [fork](https://help.github.com/articles/fork-a-repo) the repository to your github account.
2. You should add a folder named after your team and put in it a text file `README`
   listing all participants
   and comments on your how to setup your solution if needed (optional).
3. After finishing the task, send a [pull request](https://help.github.com/articles/using-pull-requests).

Submission deadline: 00:00 14 Aug
