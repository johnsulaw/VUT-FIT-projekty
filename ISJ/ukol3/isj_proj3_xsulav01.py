#!/usr/bin/env python3

import re

# ukol za 2 body
def first_odd_or_even(numbers):
    """Returns 0 if there is the same number of even numbers and odd numbers
       in the input list of ints, or there are only odd or only even numbers.
       Returns the first odd number in the input list if the list has more even
       numbers.
       Returns the first even number in the input list if the list has more odd 
       numbers.

    >>> first_odd_or_even([2,4,2,3,6])
    3
    >>> first_odd_or_even([3,5,4])
    4
    >>> first_odd_or_even([2,4,3,5])
    0
    >>> first_odd_or_even([2,4])
    0
    >>> first_odd_or_even([3])
    0
    """
    odd_list = []
    even_list = []

    for i in numbers:
        if (i % 2) == 0:
            even_list.append(i)
        else:
            odd_list.append(i)

    if ((len(even_list) == 0) or (len(odd_list) == 0)) or len(even_list) == len(odd_list):
        return 0
    elif len(odd_list) > len(even_list):
        return even_list[0]
    elif len(even_list) > len(odd_list):
        return odd_list[0]






# ukol za 3 body
def to_pilot_alpha(word):
    """Returns a list of pilot alpha codes corresponding to the input word

    >>> to_pilot_alpha('Smrz')
    ['Sierra', 'Mike', 'Romeo', 'Zulu']
    """

    pilot_alpha = ['Alfa', 'Bravo', 'Charlie', 'Delta', 'Echo', 'Foxtrot',
        'Golf', 'Hotel', 'India', 'Juliett', 'Kilo', 'Lima', 'Mike',
        'November', 'Oscar', 'Papa', 'Quebec', 'Romeo', 'Sierra', 'Tango',
        'Uniform', 'Victor', 'Whiskey', 'Xray', 'Yankee', 'Zulu']

    uppercase_word = word.upper()
    dictionary = {}

    for code in pilot_alpha:
        dictionary[code[0]] = ""
        dictionary[code[0]] = code

    pilot_alpha_list = []

    for i in uppercase_word:
        pilot_alpha_list.append(dictionary.get(i))
     
    return pilot_alpha_list


if __name__ == "__main__":
    import doctest
    doctest.testmod()
