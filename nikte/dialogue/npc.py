#!/usr/bin/env python
# -*- coding: utf-8 -*-

import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize, sent_tokenize
from enum import Enum

class NPCState(Enum):
    NONE = 0
    GREETED = 1

class NPC:
    # NPC's name
    name = ''

    # Main NPC state
    state_stack = [NPCState.NONE]

    # What the NPC says if it doesn't understand the first input
    engaging_tries_stack = ["...", "Sorry, did you talk to me?"]

    # What counts as conversastion openers from the player input
    greetings = ["hi", "hey", "hello", "heya", "hiho"]

    # What counts as conversation closers from the player input
    farewells = ["bye", "see ya", "see you", "goodbye", "good bye"]

    # If > 0, the NPC will be focused to the player for a certain amount of time
    attention_timer = 0

    # Constructor
    def __init__(self, name='Eliza'):
        self.name = name

    # Update NPC's actions independently of the player
    def update_current_action(self):
        print('*' + self.name + ' is walking randomly...*')

    # Process player input
    def process_input(self, request):
        if self.state_stack[-1] == NPCState.NONE:
            response = self.process_when_none(request)

        elif self.state_stack[-1] == NPCState.GREETED:
            response = self.process_when_greeted(request)

        if response:
            return response

    # PROCESS STATE WHEN NOT FOCUSED
    def process_when_none(self, request):
        if request:
            if request in self.greetings:
                self.state_stack.append(NPCState.GREETED)
                return 'Hi! I\'m ' + self.name + ', how can I help you?'
            if self.engaging_tries_stack:
                self.attention_timer = 100
                return self.engaging_tries_stack.pop()
        self.update_current_action()

    # PROCESS STATE WHEN GREETED
    def process_when_greeted(self, request):
        clicked_on_phrase = False # If player hasn't typed the answer, but instead clicked on the gui

        # If the player finishes the conversation
        if request in self.farewells:
            self.state_stack.pop()
            return "Bye!"

        # Select response directly from database
        if clicked_on_phrase:
            pass

        # NLP module
        else:
            return 'greet'

# def contains_words(sentence, words):
    # tokens = word_tokenize(sentence.lower())
    # for word in words:
        # if word in tokens:
            # return True
    # return False

# # Objective: generalize conversation situations
# # If talks too much, NPC says bye
# # If she doesn't know you, she asks your name
# # If you don't say hi she will find you weird
# class Npc:
    # name = ""
    # knowledge = {}
    # greeted = False
    # greet_try_stack = ["...", "Sorry, did you talk to me?"]
    # greetings = ["hi", "hey", "hello", "heya", "hiho"]
    # information_to_receive = ""
    # answer_to_information = ""
    # conversation = {
        # "job" : "I'm a peasant. I'm tired of passing the whole day under the sun!" 
    # }

    # def __init__(self, name):
        # self.name = name

    # def answer(self, sentence):
        # if self.information_to_receive:
            # self.knowledge[self.information_to_receive] = sentence
            # self.information_to_receive = ""
            # return self.answer_to_information % sentence

        # # If player has not started the conversation yet
        # if not self.greeted:
            # if contains_words(sentence, self.greetings):
                # self.greeted = True
                # if "name" in self.knowledge:
                    # return "Hiiiii, " + self.knowledge["name"] + ". How are you doing? :)"
                # else:
                    # self.information_to_receive = "name"
                    # self.answer_to_information = "%s! Nice to meet you :D How can I help you?"
                    # return "Hello! I don't think I know you... What's your name?"
            # if len(self.greet_try_stack) == 0:
                # return ""
            # return self.greet_try_stack.pop()

        # # Analizing conversation
        # else:
            # # stop_words = set(stopwords.words('english'))
            # words = nltk.word_tokenize(sentence)
            # # words = [w for w in words if not w in stop_words]
            # tagged = nltk.pos_tag(words)

            # suj = ''
            # vb = ''
            # obj = ''
            # for t in tagged:
                # if t[1] == 'PRP':
                    # suj = t[0]
                # elif t[1] == 'VB':
                    # vb = t[0]
                # elif t[1] == 'NN' or t[1] == 'NNS':
                    # obj = t[0]
                # print(t)
            # if len(vb) > 0 and len(obj) > 0:
                # return "Ah, so you want to " + vb + " " + obj
            # elif len(obj) > 0:
                # return "What about " + obj + "?"

            # # Default phrase
            # return "I see..."

    # def bye(self):
        # if not self.greeted:
            # return "... Bye?"
        # self.greeted = False
        # return "Okay, bye. See you around :)"

# # itze = Npc("Itzé")
