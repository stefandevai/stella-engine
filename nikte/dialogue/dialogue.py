#!/usr/bin/env python
# -*- coding: utf-8 -*-

from npc import NPC

def chat():
    npc = NPC()
    sentence = input('Say something to ' + npc.name + ': ')
    while sentence != 'quit':
        eliza_speech = npc.process_input(sentence)
        if eliza_speech:
            print(npc.name.upper() + ': ' + eliza_speech)
        sentence = input('YOU: ')

def main():
    chat()

if __name__ == "__main__":
    main()
