'''
Module containing classes and functions to work with Romanian license plate
'''

#de vazut daca county e valid, numerele sunt valide

class ROLicensePlate:
    '''
    TODO: Add validation for license plate strings
    '''
    
    def __init__(self,license_str: str) -> None:
        '''
        Initialise a ROLicentaPlate instance from a license number given as a string.
        License string may or may not contain spaces.
        '''
        self.county=""
        self.numbers=""
        self.letters=""

        #1) Normalize inputs by removing spaces
        #2) Iterate character by character over the string
        #   3) First letters will be the county code
        #   4)After county code, you have numbers
        #   5)After the numbers, we have the letters
        
        license_str=license_str.replace(" ","")
        
        found_county=False
        found_digit=False
        wrong_digit=False
        wrong_char=False
        
        for i in license_str:
            if i.isalpha():
                if not found_county:
                    self.county +=i
                else:
                    found_digit=True
                    self.letters +=i
            elif i.isdigit():
                found_county=True
                self.numbers +=i
                if found_digit:
                    wrong_digit=True
            else:
                wrong_char=True
        
        if self.numbers!="" and wrong_digit==False and wrong_char==False:
            self.numbers=int(self.numbers)
        else:
            self.numbers=0
            
        
        
    def validare(self) ->bool:
        if self.county=="" or self.numbers==0 or self.letters=="" or not self.county.isupper() or not self.letters.isupper():
            return False
        elif self.numbers<0 or self.numbers>999:
            return False
        elif self.letters[0]=='I' or self.letters[0]=='O' or len(self.letters)!=3 or "Q" in self.letters:
            return False
        elif self.county!="B":
            if len(self.county)!=2:
                return False
        
        return True
        
    def __repr__(self):
        return f"<ROLicensePlate county={self.county}, numbers={self.numbers}, letters={self.letters}"
    
    
    
    
    
    
    
    