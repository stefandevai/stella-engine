#!/usr/bin/env python
# -*- coding: utf-8 -*-

class Npc:
    name = ""
    knowledge = {}
    greeted = False
    greet_try_stack = ["...", "Sorry, did you talk to me?"]
    greetings = ["hi", "hey", "hello", "heya", "hiho"]
    conversation = {
        "job" : "I'm a peasant. Can't you see?"
    }

    def __init__(self, name):
        self.name = name

    def answer(self, sentence):
        if not self.greeted:
            if sentence.lower() in self.greetings:
                self.greeted = True
                if "name" in self.knowledge:
                    return "Heya " + self.knowledge["name"] + ". How are you doing?"
                else:
                    return "Hiiiiiii! Nice to meet you :)"
            if len(self.greet_try_stack) == 0:
                return ""
            return self.greet_try_stack.pop()
        if sentence in self.conversation:
            return self.conversation[sentence]
        return "I see..."

    def bye(self):
        return "See you around :)"

itze = Npc("Itzé")
