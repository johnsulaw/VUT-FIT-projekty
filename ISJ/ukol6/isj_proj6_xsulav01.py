#!/usr/bin/env python3


"""Trida polynomu implementovana jako list koeficientu"""


class Polynomial:
    """Inicializace listu s koeficienty na zaklade typu argumentu"""
    def __init__(self, *args, **kwargs):
        self.polynomial = list()
        for arg in args:    # jestlize je argument pole
            if isinstance(arg, list):
                self.polynomial = arg
                return
        if args:            # argumenty jsou integery
            self.polynomial = list(args)
        else:               # argumenty jsou predavany v parech jmeno koeficientu=hodnota
            for name, value in kwargs.items():
                for x in range(int(name.split("x")[-1]) - len(self.polynomial) + 1):
                    self.polynomial.append(0)
                # nacti hodnoty na spravne indexy
                self.polynomial[int(name.split("x")[-1])] = value
        # cyklus pro smazani prebytecnych nul
        for i in range(len(self.polynomial) - 1, 0, -1):  # prohledavej od konce
            if self.polynomial[i] != 0:  # nenulova hodnota znamena preruseni cyklu
                return
            else:
                self.polynomial.pop(i)

    """Funkce pro vypis polynomu podle zadani"""
    def __str__(self):
        polynomial = ""  # retezec pro vypis, ktery se bude vracet
        polynomial_pow = len(self.polynomial) - 1   # pocitadlo mocniny koeficientu
        # jestlize je v polynomu pouze absolutni clen
        if len(self.polynomial) == 1:
            return str(self.polynomial[0])
        for x in reversed(self.polynomial):
            if x == 0:              # pokud je koeficient roven 0, preskakuje se
                polynomial_pow -= 1
                continue
            if polynomial:          # pridani znamenka podle hodnoty koeficientu
                if x > 0:
                    polynomial += " + "
                else:
                    polynomial += " - "
            if polynomial_pow == 0:  # pripsani absolutniho clene
                polynomial += str(abs(x))
                continue
            if polynomial_pow == 1:  # koeficienty na prvni se zapisi jako x
                if abs(x) == 1:
                    polynomial += "x"
                else:
                    polynomial += str(abs(x)) + "x"
                polynomial_pow -= 1
                continue
            if abs(x) == 1:         # pokud je hodnota koeficientu 1, neni treba ji pred x vypisovat
                if polynomial_pow == 1:
                    polynomial += "x"
                else:
                    polynomial += "x^" + str(polynomial_pow)
            else:
                polynomial += str(abs(x)) + "x^" + str(polynomial_pow)
            polynomial_pow -= 1     # dekrementace pocitadla stupne polynomu

        return polynomial

    """Funkce pro porovnani dvou polynomu"""
    def __eq__(self, other):
        # polynomy musi byt stejne dlouhe, aby se rovnaly
        if len(self.polynomial) == len(other.polynomial):
            for s_member, o_member in zip(self.polynomial, other.polynomial):
                # porovnavani clenu obou polynomu
                if s_member != o_member:
                    return False
            return True
        else:
            return False

    """Funkce pro scitani dvou polynomu"""
    def __add__(self, other):
        result = self.polynomial[:]  # pomocne pole vytvorene z argumentu self
        i = 0
        # scitani koeficientu se stejnou mocninou
        for x, y in zip(self.polynomial, other.polynomial):
            result[i] = x + y
            i += 1
        # jestlize se lisi delka polynomu, tak se do pole s vysledkem pridaji chybejici koeficienty
        if len(self.polynomial) < len(other.polynomial):
            for i in range(len(self.polynomial), len(other.polynomial)):
                result.append(other.polynomial[i])
        return Polynomial(result)

    """Funkce pro nasobeni dvou polynomu"""
    def __mul__(self, other):
        # vytvoreni pole pro vysledek nasobeni
        result = [0] * (1 + len(self.polynomial) + len(other.polynomial))
        for x in range(len(self.polynomial)):
            for y in range(len(other.polynomial)):
                result[x + y] += self.polynomial[x] * other.polynomial[y]
        return Polynomial(result)

    """Funkce pro umocneni dvou polynomu"""
    def __pow__(self, p):
        if p == 1:  # prvni mocnina polynomu = polynom
            return Polynomial(self.polynomial)
        if p == 0:  # nulta mocnina polynomu = 1
            return 1
        if p > 1:   # umocneni polynomu hodnotou z argumentu p
            result = self
            for i in range(1, p):
                result *= self
            return Polynomial(result)

    """Funkce pro derivaci polynomu"""
    def derivative(self):
        if len(self.polynomial) == 1:
            # derivace konstanty = 0
            return 0
        # pole pro vysledek derivace
        # bez absolutniho clenu, protoze derivace konstanty = 0
        result = self.polynomial[1:]
        # provedeni derivace
        for x in range(len(result)):
            result[x] *= (x+1)
        return Polynomial(result)

    """Funkce vycisli polynom pro zadane x
       pokud jsou zadany dva parametry, vraci se rozdil jejich vysledku (integral)"""
    def at_value(self, x1, x2=None):
        result1 = self.polynomial[-1]
        for i in reversed(range(len(self.polynomial) - 1)):
            result1 = (result1*x1)+self.polynomial[i]
        if x2 is None:  # jestlize je zadan pouze jeden parametr, vrat vysledek
            return result1
        # jestlize je zadan druhy parametr, opakuj postup vypoctu a vrat rozdil vysledku
        result2 = self.polynomial[-1]
        for i in reversed(range(len(self.polynomial) - 1)):
            result2 = (result2*x2)+self.polynomial[i]
        return result2 - result1
