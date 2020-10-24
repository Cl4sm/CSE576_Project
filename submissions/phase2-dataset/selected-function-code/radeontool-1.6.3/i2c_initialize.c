void i2c_initialize(void)
{
    int tmp;

    tmp = GET_REG(GPIO_OUT - 0x8);
    SET_REG(GPIO_OUT - 0x8, tmp);
    tmp = GET_REG(GPIO_OUT - 0x8) | 0x101;
    SET_REG(GPIO_OUT - 0x8, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0x100);
    SET_REG(GPIO_OUT, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0x101);
    SET_REG(GPIO_OUT, 0x0);
    tmp = GET_REG(GPIO_OUT - 0x4) & (~0x100);
    SET_REG(GPIO_OUT - 0x4, tmp);
    tmp = GET_REG(GPIO_OUT - 0x4) & (~0x101);
    SET_REG(GPIO_OUT - 0x4, 0x0);
    tmp = GET_REG(GPIO_OUT) & (~0x100);
    SET_REG(GPIO_OUT, tmp);
    tmp = GET_REG(GPIO_OUT) & (~0x101);
    SET_REG(GPIO_OUT, 0x0);
}