'''
Module containing classes and functions to work with Romanian license plates.
'''

class RoLicensePlate:
    '''
    TO DO: Add validation for license plate strings
    '''
    def __init__(self, license_str:str) -> None:
        '''
        Initializes a RoLicensePlate instance from a license number given as string.
        License string may or may not contain spaces.
        '''
        self.county = ""
        self.numbers = ""
        self.letters = ""
        
        # 1) Normalize inputs by removing spaces
        # 2) Iterate character by character over the string
        # 3) First letters will be the county code
        # 4) After the county code, we have the numbers
        # 5) After the numbers, we have the letters
        
        license_str = license_str.replace(" ","") # replace white spaces with empty strings to eliminate them
        # print(license_str)
        
        # flags waiting to be set
        found_county = False
        self.leading_zero = False
        self.number_is_wrong = False
        
        # extracting info from the input string
        for i in license_str:
            if i.isalpha():
                if not found_county:
                    self.county += i
                else:
                    self.letters += i
            elif i.isdigit():
                found_county = True
                self.numbers += i
        
        # setting flags
        if len(self.numbers) > 0:  # we want to convert our numbers to int, so we have to have something to convert (len>0)
            if self.numbers[0] == "0":
                self.leading_zero = True
            if len(self.numbers) > 1 and self.numbers[1] == "0" and self.leading_zero: # number is clearly wrong if it has 2 leading zeros
                self.number_is_wrong = True
            self.numbers = int(self.numbers)
        
    def __repr__(self): # aici mai trebuie vazut cum afisam numerele de 3 cifre pt B
        numbers_repr = self.numbers
        number_is_zero = (numbers_repr == 0)
        if self.leading_zero: # if the number has a leading zero we will write it
            numbers_repr = str(numbers_repr)
            numbers_repr = "0" + numbers_repr
        if self.number_is_wrong: # if the number has 2 leading zeros, then
            if not number_is_zero: # we will write another 0, only if the number is not 0, else it would already have that 0 written
                numbers_repr = "0" + numbers_repr
        return f"RoLicensePlate county={self.county}, numbers={numbers_repr}, letters={self.letters}"
    
    def validate_county(self):
        county_str_length = len(self.county)
        if county_str_length > 2 or county_str_length < 1: # only len = 1 and len = 2 are ok
            return False
        if county_str_length == 1: # if len = 1, then it has to be "B"
            if self.county != "B":
                return False
        else:                       # if len = 2, then the county code should be one of these
            if self.county not in ["AB","AG","AR","BC","BH","BN","BR","BT","BV","BZ",
                                   "CJ","CL","CS","CT","CV","DB","DJ","GJ","GL","GR",
                                   "HD","HR","IF","IL","IS","MH","MM","MS","NT","OT",
                                   "PH","SB","SJ","SM","SV","TL","TM","TR","VL","VN","VS"]:
                return False
        return True
    
    def validate_numbers(self):
        numbers_str = str(self.numbers) # converting number back to string so we can operate with it in its real form (captured by camera)
        if self.number_is_wrong: # if the number has 2 leading zeros it is clearly wrong
            return False
        if self.leading_zero: # if it has only one leading zero, then we consider it as part of the number
            numbers_str = "0" + numbers_str
        numbers_str_length = len(numbers_str) # we take the length of the real number as shown on the license plate
        if numbers_str_length > 3 or numbers_str_length < 2: # only len = 2 or len = 3 are fine
            return False
        if numbers_str_length == 3: # if len = 3, then it should only belong to the "B" county code, else it's wrong
            if self.county != "B" or self.leading_zero:
                return False
        return True
    
    def validate_letters(self):
        return len(self.letters) == 3 # only len = 3 is ok for every license plate
    
    def validate_license_plate(self): # a license plate is valid only if each part of it is valid
        return self.validate_county() and self.validate_numbers() and self.validate_letters()