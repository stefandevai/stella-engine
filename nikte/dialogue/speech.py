#!/usr/bin/env python
# -*- coding: utf-8 -*-

# import nltk
# from nltk.corpus import stopwords
# from nltk.tokenize import word_tokenize, sent_tokenize
from enum import Enum

# What counts as conversastion openers from the player input
GREETINGS = ["hi", "hey", "hello", "heya", "hiho"]
# What counts as conversation closers from the player input
FAREWELLS = ["bye", "see ya", "see you", "goodbye", "good bye"]

# Data structure for holding a speech in database
class Speech:
    sid = ''
    verbs = []
    subjects = []
    pronouns = []
    keywords = []
    pos_keywords = {}
    responses = []
    next_speech = ''
    times_said = 0

    def __init__(self, sid):
        self.sid = sid

speech1 = Speech('ELZ001')
speech1.keywords = GREETINGS
speech1.responses = ['Heya, stranger!', 'Hi! How are you doing?']
speech1.next_speech = 'NON_ATTACHED'

speech2 = Speech('ELZ002')
speech2.keywords = ['encomienda']
speech2.responses = ['I\'m tired of paying this useless tribute to the encomendero!']
speech2.next_speech = 'ELZ003'
speech2.pos_keywords = {
    'NOUN': ['encomienda']
}

speech3 = Speech('ELZ003')
speech3.keywords = ['tribute', 'pay']
speech3.responses = ['Yeah, we pay it because they said we would be better off than in the ancient times, but look what happened!']
speech3.next_speech = 'NON_ATTACHED'
speech3.pos_keywords = {
    'NOUN': ['tribute'],
    'VERB': ['pay'],
}

speech4 = Speech('ELZ004')
speech4.keywords = ['sad', 'sadness', 'cry']
speech4.responses = ['It\'s just that I can\'t believe what happened to our beautiful island...']
speech4.next_speech = 'ELZ005'
speech4.pos_keywords = {
    'ADJ': ['sad'],
    'VERB': ['cry'],
}

speech5 = Speech('ELZ005')
speech5.keywords = ['island', 'isla']
speech5.responses = ['Everything changed when they arrived... now they say it\'s their encomienda.']
speech5.next_speech = 'ELZ002'
speech5.pos_keywords = {
    'NOUN': ['island'],
    'VERB': ['happen'],
}

SPEECH_DATABASE = {
    'eliza' : {
        'NON_ATTACHED' : [speech1, speech4],
        speech2.sid : [speech2],
        speech3.sid : [speech3],
        speech5.sid : [speech5]
    }
}

