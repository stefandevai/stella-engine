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
            next_speech = 'ELZ005',
            pos_keywords = {
                ADJ = {'sad'},
                VERB = {'cry'}
            }
        }
    },

    ELZ2 = {{
        keywords = {'encomienda'},
        responses = {'I\'m tired of paying this useless tribute to the encomendero!'},
        next_speech = 'ELZ003',
        pos_keywords = {
            NOUN = {'encomienda'}
        }
    }},

    ELZ3 = {{
        keywords = {'tribute', 'pay'},
        responses = {'Yeah, we pay it because they said we would be better off than in'},
        next_speech = 'NON_ATTACHED',
        pos_keywords = {
            NOUN = {'tribute'},
            VERB = {'pay'}
        }   
    }},

    ELZ5 = {{
        keywords = {'island', 'isla'},
        responses = {'Everything changed when they arrived... now they say it\'s their encomienda'},
        next_speech = 'ELZ002',
        pos_keywords = {
            NOUN = {'island'},
            VERB = {'happen'}
        }        
    }}
}