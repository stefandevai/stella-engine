#!/usr/bin/env python
# -*- coding: utf-8 -*-

from nltk import word_tokenize

def contains_words(sentence, words):
    tokens = word_tokenize(sentence.lower()) 
    for word in words:
        if word in tokens:
            return True
    return False

# Objective: generalize conversation situations
# If talks too much, NPC says bye
# If she doesn't know you, she asks your name
# If you don't say hi she will find you weird
class Npc:
    name = ""
    knowledge = {}
    greeted = False
    greet_try_stack = ["...", "Sorry, did you talk to me?"]
    greetings = ["hi", "hey", "hello", "heya", "hiho"]
    information_to_receive = ""
    answer_to_information = ""
    conversation = {
        "job" : "I'm a peasant. I'm tired of passing the whole day under the sun!" 
    }

    def __init__(self, name):
        self.name = name

    def answer(self, sentence):
        if self.information_to_receive:
            self.knowledge[self.information_to_receive] = sentence
            self.information_to_receive = ""
            return self.answer_to_information % sentence

        if not self.greeted:
            if contains_words(sentence, self.greetings):
                self.greeted = True
                if "name" in self.knowledge:
                    return "Hiiiii, " + self.knowledge["name"] + ". How are you doing? :)"
                else:
                    self.information_to_receive = "name"
                    self.answer_to_information = "%s! Nice to meet you :D How can I help you?"
                    return "Hello! I don't think I know you... What's your name?"
            if len(self.greet_try_stack) == 0:
                return ""
            return self.greet_try_stack.pop()
        if sentence.lower() in self.conversation:
            return self.conversation[sentence.lower()]
        return "I see..."

    def bye(self):
        if not self.greeted:
            return "... Bye?"
        self.greeted = False
        return "Okay, bye. See you around :)"

itze = Npc("Itzé")
