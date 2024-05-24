from validareFinalaEchipa3 import RoLicensePlate

license_plate = RoLicensePlate("TM 07 ABC")
print(license_plate)
print(license_plate.validate_license_plate())
#TRUE

license_plate2 = RoLicensePlate("TM 99 ABC")
print(license_plate2)
print(license_plate2.validate_license_plate())
#TRUE

license_plate3 = RoLicensePlate("TMS 99 ABCD")
print(license_plate3)
print(license_plate3.validate_license_plate())
#FALSE

license_plate4 = RoLicensePlate("T 99 ABCD")
print(license_plate4)
print(license_plate4.validate_license_plate())
#FALSE

license_plate5 = RoLicensePlate("B 001 ABC")
print(license_plate5)
print(license_plate5.validate_license_plate())
#FALSE

license_plate6 = RoLicensePlate("TM 100 ABC")
print(license_plate6)
print(license_plate6.validate_license_plate())
#FALSE

license_plate7 = RoLicensePlate("TM 00 ABC")
print(license_plate7)
print(license_plate7.validate_license_plate())
#FALSE

license_plate8 = RoLicensePlate("TM 001 ABC")
print(license_plate8)
print(license_plate8.validate_license_plate())
#FALSE

license_plate9 = RoLicensePlate("TM 099 ABC")
print(license_plate9)
print(license_plate9.validate_license_plate())
#FALSE

license_plate10 = RoLicensePlate("TM 05 AB")
print(license_plate10)
print(license_plate10.validate_license_plate())
#FALSE

license_plate11 = RoLicensePlate("TM 05 ABCD")
print(license_plate11)
print(license_plate11.validate_license_plate())
#FALSE

license_plate12 = RoLicensePlate("B 005 ABCD")
print(license_plate12)
print(license_plate12.validate_license_plate())
#FALSE

license_plate13 = RoLicensePlate("B 055 ABC")
print(license_plate13)
print(license_plate13.validate_license_plate())
#FALSE

license_plate14 = RoLicensePlate("B 55 ABC")
print(license_plate14)
print(license_plate14.validate_license_plate())
#TRUE

license_plate15 = RoLicensePlate("B 155 ABC")
print(license_plate15)
print(license_plate15.validate_license_plate())
#TRUE

license_plate16 = RoLicensePlate("B 05 ABC")
print(license_plate16)
print(license_plate16.validate_license_plate())
#TRUE

license_plate17 = RoLicensePlate("B 175 ABCD")
print(license_plate17)
print(license_plate17.validate_license_plate())
#FALSE

license_plate18 = RoLicensePlate("B 175 ABCDEFG")
print(license_plate18)
print(license_plate18.validate_license_plate())
#FALSE

license_plate19 = RoLicensePlate("BC 175 ABC")
print(license_plate19)
print(license_plate19.validate_license_plate())
#FALSE

license_plate20 = RoLicensePlate("BC 075 ABC")
print(license_plate20)
print(license_plate20.validate_license_plate())
#FALSE

license_plate21 = RoLicensePlate("BC 75 ABC")
print(license_plate21)
print(license_plate21.validate_license_plate())
#TRUE

license_plate22 = RoLicensePlate("B 01 1ABC")
print(license_plate22)
print(license_plate22.validate_license_plate())
#FALSE

license_plate23 = RoLicensePlate("TM 1 1ABC") # e ok i guess
print(license_plate23)
print(license_plate23.validate_license_plate())
#TRUE

license_plate24 = RoLicensePlate("TM 11 123")
print(license_plate24)
print(license_plate24.validate_license_plate())
#FALSE

license_plate25 = RoLicensePlate("TM 11123")
print(license_plate25)
print(license_plate25.validate_license_plate())
#FALSE (bc if it is a red one it should begin with 0)

license_plate26 = RoLicensePlate("TM 011123")
print(license_plate26)
print(license_plate26.validate_license_plate())
#TRUE

license_plate27 = RoLicensePlate("B 011123")
print(license_plate27)
print(license_plate27.validate_license_plate())
#TRUE

license_plate28 = RoLicensePlate("B 045")
print(license_plate28)
print(license_plate28.validate_license_plate())
#TRUE

license_plate29 = RoLicensePlate("TM 01")
print(license_plate29)
print(license_plate29.validate_license_plate())
#FALSE

license_plate30 = RoLicensePlate("B 01")
print(license_plate30)
print(license_plate30.validate_license_plate())
#FALSE

license_plate31 = RoLicensePlate("TM 0111234")
print(license_plate31)
print(license_plate31.validate_license_plate())
#FALSE

license_plate32 = RoLicensePlate("CD 146 108")
print(license_plate32)
print(license_plate32.validate_license_plate())
#TRUE

license_plate33 = RoLicensePlate("CO 129 108")
print(license_plate33)
print(license_plate33.validate_license_plate())
#FALSE

license_plate33 = RoLicensePlate("CO 130 108")
print(license_plate33)
print(license_plate33.validate_license_plate())
#TRUE

license_plate34 = RoLicensePlate("TC 129 008")
print(license_plate34)
print(license_plate34.validate_license_plate())
#FALSE

license_plate35 = RoLicensePlate("TC 12 218")
print(license_plate35)
print(license_plate35.validate_license_plate())
#FALSE

license_plate36 = RoLicensePlate("TM 12 ION")
print(license_plate36)
print(license_plate36.validate_license_plate())
#FALSE

license_plate37 = RoLicensePlate("TM 12 III")
print(license_plate37)
print(license_plate37.validate_license_plate())
#FALSE

license_plate38 = RoLicensePlate("TM 12 NIO")
print(license_plate38)
print(license_plate38.validate_license_plate())
#TRUE

license_plate39 = RoLicensePlate("TM 2A 3BC")
print(license_plate39)
print(license_plate39.validate_license_plate())
#FALSE

license_plate40 = RoLicensePlate(" B58 PKW")
print(license_plate40)
print(license_plate40.validate_license_plate())