#!/usr/bin/env python
# -*- coding: utf-8 -*-

from enum import Enum
import spacy
import speech

class NPCState(Enum):
    NONE = 0
    GREETED = 1

class NPC:
    # NPC's name
    name = ''
    # Main NPC state
    state_stack = [NPCState.NONE]
    # What the NPC says if it doesn't understand the first input
    engaging_tries_stack = ["Did you talk to me?", "..."]
    # Times the NPC tried to engage in contact
    engaging_tries = 0
    # If > 0, the NPC will be focused to the player for a certain amount of time
    attention_timer = 0
    # Current speech id
    current_speech = 'NON_ATTACHED'

    # Constructor
    def __init__(self, name='Eliza'):
        self.name = name
        self.nlp = spacy.load("en_core_web_sm")

    # Update NPC's actions independently of the player
    def update_current_action(self):
        print('*' + self.name + ' is crying in a dark corner...*')

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
            if request.lower() in speech.GREETINGS:
                self.state_stack.append(NPCState.GREETED)
                # return 'Hi! I\'m ' + self.name + ', how can I help you?'
                return 'Hi, Nikté... :\'('
            if self.engaging_tries < len(self.engaging_tries_stack):
                response = self.engaging_tries_stack[self.engaging_tries]
                self.engaging_tries = self.engaging_tries + 1
                self.attention_timer = 100
                return response
        self.update_current_action()

    # PROCESS STATE WHEN GREETED
    def process_when_greeted(self, request):
        clicked_on_phrase = False # If player hasn't typed the answer, but instead clicked on the gui

        # If the player finishes the conversation
        if request in speech.FAREWELLS:
            self.state_stack.pop()
            self.engaging_tries = 0
            self.current_speech = 'NON_ATTACHED'
            return "Bye"

        # Select response directly from database
        if clicked_on_phrase:
            pass

        else:
            # First check if there are any keywords that match
            sps = speech.SPEECH_DATABASE[self.name.lower()][self.current_speech]

            for s in sps:
                if request in s.keywords:
                    self.current_speech = s.next_speech
                    s.times_said = s.times_said + 1
                    response = s.responses[0]

                    if s.times_said > 1:
                        response = 'As I said, ' + response
                    return response

            # NLP module
            response = self.nlp_process(request)
            if response:
                return response

    def nlp_process(self, request):
        response = ''
        sps = speech.SPEECH_DATABASE[self.name.lower()][self.current_speech]

        for s in sps:
            doc = self.nlp(request)

            for token in doc:
                print(token.text, token.lemma_, token.pos_, token.tag_, token.dep_, token.is_alpha, token.is_stop)
                if token.pos_ in s.pos_keywords:
                    if token.lemma_ in s.pos_keywords[token.pos_]:
                        self.current_speech = s.next_speech
                        response = s.responses[0]
        return response

