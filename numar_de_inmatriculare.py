'''
Module containig classes and functions to work with Romanian licence plates
'''
lista_judete= [  "AB", "AR" , "AG" , "BC" , "BH" , "BN" , "BT" , "BV" , "BR" , "BZ"
               , "CS" , "CL" , "CJ" , "CT" , "CV" , "DB" , "DJ" , "GL" , "GR" , "GJ"
               , "HR" , "HD" , "IL" , "IS" , "IF" , "MM" , "MH" , "MS" , "NT" , "OT"
               , "PH" , "SM", "SJ" , "SB" , "SV" , "TR" , "TM" , "TL" , "VS" , "VL" 
               , "VN", "B"]
lista_nr_speciale=[101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,
                   116, 122, 123, 124, 125, 126, 127, 128, 130, 131, 134, 136, 138, 141, 142,
                   146, 150, 152, 154, 155, 156, 157, 159, 165, 166, 167, 168, 170, 183, 189,
                   191, 193, 205, 206, 207, 210, 211, 214, 216, 217, 220, 222, 223, 226, 234]
lista_corpuri=["CO", "CD", "TC"]


class RoLicensePlate():
    
    
    
    def __init__(self,  licence_string:str) -> None:
        '''
        initialies a RoLIcensePlate instance from a licence number given as string
        numarul poate sa contina sau nu spatii
        
        TEMA!!
        '''
        self.county = ""
        self.numbers =""
        self.letters = ""
        self.normal_license=False
        #facem ca tot inputul sa nu aiba spatii
        #iteram caracter cu caracter pana ajungem la o cifra
        #   prima litera o sa faca parte din judet
        #   dupa judet urmeaza numarul
        #   ajungi dupa la literele random
        
        licence_string=licence_string.replace(" ", "")
        #print(licence_string)
        
        found_county=False
        
        for i in licence_string:
            if i.isalpha():
                if not found_county:
                    self.county += i 
                else:
                    self.letters += i
                    self.normal_license=True
                    #daca avem litere la final, adica daca este un numar negru
                    
            elif i.isdigit():
                found_county=True
                self.numbers += i
            
             
        #self.numbers=int(self.numbers)
    def check_numar_diplomatic(self):
        if self.county in lista_corpuri:
            if len(self.numbers)==6 and self.normal_license is False:
                tara=int(int(self.numbers)/1000)
                if tara in lista_nr_speciale:
                    return True
        return False
    
    def check_numar_rosu(self):
        if self.county in lista_judete and self.normal_license is False:
            if len(self.numbers)>=3 and len(self.numbers)<=6:
                if self.numbers[0]=='0' and self.numbers[1]!='0':
                    return True
        return False
            
    def check_county(self):
        if self.county in lista_judete:
            return True
        else:
            return False
    
    def check_letters(self):
        if self.normal_license:
            if self.letters[0]=="O" or self.letters[0]=="I":
                return False
            elif "Q" in self.letters:
                return False
            elif len(self.letters) == 3 :
                return True
            else:
                return False    
    
    def check_numbers(self):
        if len(self.numbers)<2 or len(self.numbers)>3:
            return False
        elif self.numbers=="00" or self.numbers=="000":
            return False
        elif len(self.numbers)==3 and self.numbers[0]=="0":
            return False
        elif len(self.county)==2 and len(self.numbers)==3:
            return False
        else:
            return True
        

    def check_ro_license_plate(self):
        if self.normal_license:
            return self.check_county() and self.check_letters() and self.check_numbers()
        else:
            return self.check_numar_diplomatic() or self.check_numar_rosu()
           
    def __repr__(self):
        return f"<RoLicensePlate county={self.county}, numbers={self.numbers}, letters={self.letters}>"
        
        