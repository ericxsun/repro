from pymodule.sub import inherit_check as ic

def pass_thru(value):
    print(value)
    return value

class PyExtend(ic.Base):
    # def __init__(self, func):
    #     # self.func = func
    #     pass
    def pure(self, value):
        print("py.pure={}".format(value))
        # self.func(value)
        return value
    def optional(self, value):
        print("py.optional={}".format(value))
        # self.func(value)
        return value * 100
