'''
Module containing classes and functions to work with Romanian license plate
'''

#de vazut daca county e valid, numerele sunt valide

class ROLicensePlate:
    '''
    TODO: Add validation for license plate strings
    '''
    def __init__(self, license_str: str)->None:
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
        # print(license_str)

        found_county=False

        for i in license_str:
            if i.isalpha():
                if not found_county:
                    self.county += i
                else:
                    self.letters += i
            elif i.isdigit():
                found_county=True
                self.numbers += i

        self.numbers=int(self.numbers)

    #permite sa am o reprez vizuala a clasei
    def __repr__(self):
        return f"<RoLicensePlate county={self.county}, numbers={self.numbers}, letters={self.letters}"
