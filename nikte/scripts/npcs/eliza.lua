name = "Eliza"

database = {
    NON_ATTACHED = {
        ELZ1 = {
            keywords = {"hi", "hello"},
            responses = {'Heya, stranger!', 'Hi! How are you doing?'},
            next_speech = 'NON_ATTACHED'
        },

        ELZ4 = {
            keywords = {'sad', 'sadness', 'cry'},
            responses = {'It\'s just that I can\'t believe what happened to our beautiful island...'},
            next_speech = 'ELZ5',
            pos_keywords = {
                ADJ = {'sad'},
                VERB = {'cry'}
            }
        },

        ELZ6 = {
            keywords = {"same", "sprite", "clothes"},
            responses = {'Haha, don\'t worry, that\'s just a prototype!'},
            next_speech = 'NON_ATTACHED'
        },
    },

    ELZ2 = {{
        keywords = {'encomienda'},
        responses = {'It\'s their excuse to steal us. We have to pay a tribute to be "protected". Aha!'},
        next_speech = 'ELZ3',
        pos_keywords = {
            NOUN = {'encomienda'}
        }
    }},

    ELZ3 = {{
        keywords = {'tribute', 'pay'},
        responses = {'Yeah, we pay double for all this suffering...'},
        next_speech = 'NON_ATTACHED',
        pos_keywords = {
            NOUN = {'tribute'},
            VERB = {'pay'}
        }   
    }},

    ELZ5 = {{
        keywords = {'island', 'isla', 'happen'},
        responses = {'Not so long ago, but before you were born, there was no encomienda!'},
        next_speech = 'ELZ2',
        pos_keywords = {
            NOUN = {'island'},
            VERB = {'happen'}
        }        
    }}
}
