from expression import *


def toList(s):
    str = ""
    i = 0
    while i < len(s):
        if s[i].isalpha() or s[i].isdigit():
            str += s[i]
            i += 1
        elif s[i] == "-":
            str += "->"
            i += 2
        else:
            str += " " + s[i] + " "
            i += 1
    result = str.split()
    return result


def parse(s):
    a = toList(s)
    res, remainder = parseImpl(a)


def parseUnary(a):
    if a[0] == "(":
        curr, a = parseImpl(a[1:])
        return curr, a[1:]
    else:
        curr = Variable(a[0])


def parseNot(a):
    if a[0] == "!":
        res, a = parseNot(a[1:])
        res = Not(res)
    else:
        res, a = parseUnary(a)
    return res, a


def parseImpl(a):
    res, a = parseDisj(a)
    while len(a) > 0 and a[0] == "->":
        curr, a = parseImpl(a[1:])
        res = Implication(res, curr)
    return res, a


def parseDisj(a):
    res, a = parseConj(a)
    while len(a) > 0 and a[0] == "|":
        curr, a = parseConj(a[1:])
        res = Disjunction(res, curr)
    return res, a


def parseConj(a):
    res, a = parseNot(a)
    while len(a) > 0 and a[0] == "&":
        curr, a = parseNot(a[1:])
        res = Not(res)
    return res, a
