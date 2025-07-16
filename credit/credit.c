#include <cs50.h>
#include <stdio.h>

char findProvider(long n);
bool isCreditCardNumberValid(long n);
int findLength(long n);
int getFirstDigit(int n);
int getFirstTwoDigits(long n);

int main()
{
    long creditNumber = get_long("Number: ");

    bool valid = isCreditCardNumberValid(creditNumber);

    if (valid)
    {
        char p = findProvider(creditNumber);

        if (p == 'a')
        {
            printf("%s\n", "AMEX");
        }
        else if (p == 'm')
        {
            printf("%s\n", "MASTERCARD");
        }
        else if (p == 'v')
        {
            printf("%s\n", "VISA");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

char findProvider(long creditNumber)
{
    int length = findLength(creditNumber);
    int ftd = getFirstTwoDigits(creditNumber);
    int fd = getFirstDigit(ftd);

    bool isAmex = length == 15 && (ftd == 34 || ftd == 37);
    bool isMastercard = length == 16 && ftd >= 51 && ftd <= 55;
    bool isVisa = (length == 13 || length == 16) && fd == 4;

    if (isAmex)
    {
        return 'a';
    }
    else if (isMastercard)
    {
        return 'm';
    }
    else if (isVisa)
    {
        return 'v';
    }
    else
    {
        return '_';
    }
}

bool isCreditCardNumberValid(long creditNumber)
{
    int sum = 0;
    int even = 0;

    int i = 0;

    do
    {
        int number = creditNumber % 10;
        creditNumber /= 10;

        bool isEvenIndex = i % 2 != 0;

        if (isEvenIndex)
        {
            int product = number * 2;

            do
            {
                int productDigit = product % 10;
                product /= 10;

                sum = sum + productDigit;
            }
            while (product > 0);
        }
        else
        {
            even = even + number;
        }

        i++;
    }
    while (creditNumber > 0);

    int result = sum + even;
    int lastDigit = result % 10;

    return lastDigit == 0;
}

int findLength(long number)
{
    int length = 0;

    if (number == 0)
    {
        return 1;
    }

    while (number != 0)
    {
        number /= 10;
        length++;
    }

    return length;
}

int getFirstDigit(int num)
{
    while (num >= 10)
    {
        num /= 10;
    }

    return num;
}

int getFirstTwoDigits(long num)
{
    if (num < 0)
    {
        num = -num;
    }

    while (num >= 100)
    {
        num /= 10;
    }

    return (int) num;
}
