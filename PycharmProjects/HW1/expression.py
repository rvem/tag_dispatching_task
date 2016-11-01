MOD = (10 ** 9) + 7


class Expression(object):
    hash = 0

    def __hash__(self):
        return self.hash

    def __eq__(self, other):
        return self.__hash__() == other.__hash__


class Binary(Expression):
    def __init__(self, left, right):
        self.left = left
        self.right = right
        self.rehash()

    def __str__(self):
        return "(" + self.left.__str__() + self.name + self.right.__str__() + ")"

    def rehash(self):
        self.hash = (5 * self.name.__hash__() + 13 * self.right.__hash__() ** 3 + 31 * self.right.__hash__() ** 4) % MOD

    def calc(self, left, right):
        raise NotImplementedError

    def eval(self, values):
        return self.calc(self.left.eval(values), self.right.eval(values))


class Implication(Binary):
    name = "->"

    def __init__(self, left, right):
        super().__init__(left, right)

    def calc(self, left, right):
        return (not left) or right


class Conjunction(Binary):
    name = "&"

    def __init__(self, left, right):
        super().__init__(left, right)

    def calc(self, left, right):
        return left and right


class Disjunction(Binary):
    name = "|"

    def __init__(self, left, right):
        super().__init__(left, right)

    def calc(self, left, right):
        return left and right


class Unary(Expression):
    def __init__(self, value):
        self.val = value


class Variable(Unary):
    def __init__(self, value):
        self.hash = value.__hash__() % MOD
        super().__init__(value)

    def __str__(self):
        return self.val

    def rehash(self):
        self.hash = self.val.__hash__()

    def eval(self, values):
        return values[self.val]


class Not(Unary):
    name = "!"

    def __init__(self, value):
        super().__init__(value)
        self.rehash()

    def rehash(self):
        self.hash = (5 * self.val.__hash__() ** 3 + 11 * self.name.__hash__()) % MOD

    def eval(self, values):
        return not self.val.eval(values)
