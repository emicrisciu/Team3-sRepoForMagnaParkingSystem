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
        self.license_plate_is_red = False
        
        # extracting info from the input string
        found_digit=False
        self.wrong_digit=False
        self.wrong_char=False
        
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
                    self.wrong_digit=True
            else:
                self.wrong_char=True
        
    def __repr__(self):
        return f"RoLicensePlate county={self.county}, numbers={self.numbers}, letters={self.letters}"
    
    def validate_county(self):
        county_str_length = len(self.county)
        if county_str_length > 2 or county_str_length < 1: # only len = 1 and len = 2 are ok
            return False
        if county_str_length == 1: # if len = 1, then it has to be "B"
            if self.county != "B":
                return False
        else:                       # if len = 2, then the county code should be one of these (including diplomatic plates)
            if self.county not in ["AB","AG","AR","BC","BH","BN","BR","BT","BV","BZ",
                                   "CJ","CL","CS","CT","CV","DB","DJ","GJ","GL","GR",
                                   "HD","HR","IF","IL","IS","MH","MM","MS","NT","OT",
                                   "PH","SB","SJ","SM","SV","TL","TM","TR","VL","VN","VS",
                                   "CD", "TC", "CO"]:
                return False
        return True
    
    def validate_numbers(self):
        numbers_str = self.numbers
        county_str = self.county
        numbers_str_length = len(numbers_str)
        letters_str_length = len(self.letters)
    
        if int(numbers_str) == 0:   # if the number is in [00, 000, 0000, 00000, 000000], then it is not valid
            return False
        if numbers_str_length < 2 or numbers_str_length > 6:    # numbers have to have 2 to 6 digits 
            return False
        if county_str in ["CD", "TC", "CO"]:    # if we have a diplomatic license plate
            if numbers_str_length != 6:     # it must have 6 digits following the diplomatic code and the first three digits must be on of these and the last three digits must not begin with a 0
                return False
            elif numbers_str[:3] not in ["101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111", "112", "113", "114",
                                       "115", "116", "122", "123", "124", "125", "126", "127", "128", "130", "131", "134", "136", "138",
                                       "141", "142", "146", "150", "152", "154", "155", "156", "157", "159", "165", "166", "167", "168",
                                       "170", "183", "189", "191", "193", "205", "206", "207", "210", "211", "214", "216", "217", "220",
                                       "222", "223", "226", "234"] or numbers_str[3] == '0':
                return False
        if numbers_str_length >= 3 and numbers_str_length <= 6: # we may have a red license plate here and we have to make sure it really is one
            if numbers_str[0] == '0' and numbers_str[1] != '0' and county_str not in ["CD", "TC", "CO"] and letters_str_length == 0:     # a license plate is red only if these conditions are met
                self.license_plate_is_red = True    # we set this flag because it is needed when we validate the letters
                return True
            if numbers_str_length == 3:     # if we have a 3-digit number, then it has to belong to the B county and not begin with 0 (e.g. 015 is not valid)
                if county_str != 'B' or numbers_str[0] == '0':
                    return False
        return True     # here we basically check if the number is a 2-digit number which is correct in any form
                
            
    def validate_letters(self):
        # only len = 3 is ok for every license plate, except diplomatic and red ones which don't have any letters at all
        letters_str_length = len(self.letters)
        letters_str = self.letters
        if self.county in ["CD", "TC", "CO"] or self.license_plate_is_red:
            return (letters_str_length == 0)
        else:
            return (letters_str_length == 3 and letters_str[0] not in ["I", "O"] and "Q" not in letters_str)
    
    def validate_license_plate(self):   # a license plate is valid only if each part of it is valid
        return not self.wrong_char and not self.wrong_digit and self.validate_county() and self.validate_numbers() and self.validate_letters()