void i2c_initialize_7e30(void)
{
    int tmp;

    tmp = GET_REG(GPIO_OUT - 0x8);
    SET_REG(GPIO_OUT - 0x8, tmp);
    tmp = GET_REG(GPIO_OUT - 0x8) | 0xC0000;
    SET_REG(GPIO_OUT - 0x8, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0x80000);
    SET_REG(GPIO_OUT, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0xC0000);
    SET_REG(GPIO_OUT, 0x0);
    tmp = GET_REG(GPIO_OUT - 0x4) & (~0x80000);
    SET_REG(GPIO_OUT - 0x4, tmp);
    tmp = GET_REG(GPIO_OUT - 0x4) & (~0xC0000);
    SET_REG(GPIO_OUT - 0x4, 0x0);
    tmp = GET_REG(GPIO_OUT) & (~0x80000);
    SET_REG(GPIO_OUT, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0xC0000);
    SET_REG(GPIO_OUT, 0x0);
}