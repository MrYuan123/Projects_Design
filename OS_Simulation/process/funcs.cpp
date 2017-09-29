ADDR setInteger(char process_name, int value) {
    _write(process_name, _int, sizeof(int), 1, &value);
}


