import enum


class TK(enum.Enum):
    EOF = -1
    INT = 0
    STRING = 1
    KEYWORD = 2
    PLUS = 3
    MINUS = 4
    MUL = 5
    DIV = 6
    LPAREN = 7
    RPAREN = 8
    LBRACE = 9
    RBRACE = 10
    EQ = 11
    NEQ = 12
    LT = 13
    GT = 14
    LTE = 15
    GTE = 16
    ASSIGN = 17
    COMMA = 18
    SEMICOLON = 19
    IF = 20
    ELSE = 21
    FOR = 22
    WHILE = 23
    FUNC = 24
    RETURN = 25
    IDENT = 26
    

class Token:

    def __init__(self, tk: TK, value: any) -> None:
        self.tk = tk
        self.value = value

    def __repr__(self) -> str:
        return f'Token<{self.tk}, {self.value}>'

class Tokenizer:
    __slots__ = ('code', 'l', 'cursor', )

    def __init__(self, code: str) -> None:
        self.code = code
        self.l = len(self.code)
        self.cursor = 0

    def next(self) -> Token:
        if self.cursor >= self.l:
            return Token(TK.EOF, None)
        c = self.code[self.cursor]
        if c.isdigit():
            return self.next_int()
        elif c == '+':
            self.cursor += 1
            return Token(TK.PLUS, '+')
        elif c == '-':
            self.cursor += 1
            return Token(TK.MINUS, '-')
        elif c == '*':
            self.cursor += 1
            return Token(TK.MUL, '*')
        elif c == '/':
            self.cursor += 1
            return Token(TK.DIV, '/')
        elif c == '(':
            self.cursor += 1
            return Token(TK.LPAREN, '(')
        elif c == ')':
            self.cursor += 1
            return Token(TK.RPAREN, ')')
        elif c == '{':
            self.cursor += 1
            return Token(TK.LBRACE, '{')
        elif c == '}':
            self.cursor += 1
            return Token(TK.RBRACE, '}')
        elif c == '=':
            self.cursor += 1
            if self.cursor < self.l and self.code[self.cursor] == '=':
                self.cursor += 1
                return Token(TK.EQ, '==')
            return Token(TK.ASSIGN, '=')
        elif c == ',':
            self.cursor += 1
            return Token(TK.COMMA, ',')
        elif c == ';':
            self.cursor += 1
            return Token(TK.SEMICOLON, ';')
        elif c == '>':
            self.cursor += 1
            if self.cursor < self.l and self.code[self.cursor] == '=':
                self.cursor += 1
                return Token(TK.GTE, '>=')
            return Token(TK.GT, '>')
        elif c == '<':
            self.cursor += 1
            if self.cursor < self.l and self.code[self.cursor] == '=':
                self.cursor += 1
                return Token(TK.LTE, '<=')
            return Token(TK.LT, '<')
        elif c == '!':
            self.cursor += 1
            if self.cursor < self.l and self.code[self.cursor] == '=':
                self.cursor += 1
                return Token(TK.NEQ, '!=')
            raise Exception('unknown char %c' % c)
        elif c == '"':
            return self.next_string()
        elif c.isalpha():
            return self.next_keyword()
        elif c.isspace():
            self.cursor += 1
            return self.next()
        else:
            raise Exception('unknown char %c' % c)
    
    def next_keyword(self) -> Token:
        start = self.cursor
        while self.cursor < self.l:
            c = self.code[self.cursor]
            if not c.isalpha():
                break
            self.cursor += 1
        return Token(TK.KEYWORD, self.code[start:self.cursor])

    def next_string(self) -> Token:
        self.cursor += 1
        start = self.cursor
        while self.cursor < self.l:
            c = self.code[self.cursor]
            if c == '"':
                self.cursor += 1
                break
            self.cursor += 1
        return Token(TK.STRING, self.code[start:self.cursor-1])
    
    def next_int(self) -> Token:
        start = self.cursor
        while self.cursor < self.l:
            c = self.code[self.cursor]
            if not c.isdigit():
                break
            self.cursor += 1
        return Token(TK.INT, int(self.code[start:self.cursor]))


class Statment:
    pass

class IfExpr:
    
    def __init__(self) -> None:
        pass
    

if __name__ == '__main__':
    code = '123 "hello world" abc + if else for while func return >= > < <= == != = ( ) { } , ;'
    tokenizer = Tokenizer(code)
    while True:
        token = tokenizer.next()
        print(token)
        if token.tk == TK.EOF:
            break