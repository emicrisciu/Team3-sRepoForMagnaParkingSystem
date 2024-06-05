def save_boolean_to_file(bool_value, file_path='cale_catre_fisier_cu_valoare.txt'):
    with open(file_path, 'w') as file:
        if bool_value:
            file.write("TRUE")
        else:
            file.write("FALSE")
    print(f"Saved boolean to file: {bool_value}")


#save_boolean_to_file(True) 