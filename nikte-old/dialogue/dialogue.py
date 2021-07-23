#!/usr/bin/env python
# -*- coding: utf-8 -*-

from npc import NPC
import time
import sys

def chat():
    npc = NPC()
    sentence = input('Say something to ' + npc.name + ': ')
    while sentence != 'quit':
        eliza_speech = npc.process_input(sentence)
        if eliza_speech:
            print(npc.name.upper() + ': ', end='')
            print_speech(eliza_speech)
        sentence = input('YOU: ')

def print_speech(string):
    for c in string:
        sys.stdout.write(c)
        sys.stdout.flush()
        time.sleep(.008)
    print('\n', end='')

def main():
    chat()

if __name__ == "__main__":
    main()
