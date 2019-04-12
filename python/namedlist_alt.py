# Alternative impl, motivated by: https://bitbucket.org/ericvsmith/namedlist
from collections import namedtuple

import numpy as np


def _item_property(cls, index):
    return property(
        fget=lambda self: cls.__getitem__(self, index),
        fset=lambda self, value: cls.__setitem__(self, index, value))


def namedview(name, fields):
    # Prevent mutation.
    fields = tuple(fields)

    class NamedView(object):
        def __init__(self, obj):
            assert len(obj) == len(fields)
            object.__setattr__(self, '_obj', obj)

        @staticmethod
        def get_fields():
            return fields

        def __getitem__(self, index):
            return self._obj.__getitem__(index)

        def __setitem__(self, index, value):
            self._obj.__setitem__(index, value)

        def __setattr__(self, name, value):
            if not hasattr(self, name):
                raise AttributeError("Cannot add attributes!")
            object.__setattr__(self, name, value)

        def __len__(self):
            return self._obj.__len__

        def __iter__(self):
            return self._obj.__iter__()

        def __repr__(self):
            values = [
                "{}={}".format(field, repr(self[i]))
                for i, field in enumerate(fields)]
            return "{}({})".format(name, ", ".join(values))

    NamedView.__name__ = NamedView.__qualname__ = name
    for i, field in enumerate(fields):
        setattr(NamedView, field, _item_property(NamedView, i))
    return NamedView


def test_main():
    MyView = namedview("MyView", ['a', 'b', 'c'])
    MyTuple = namedtuple("MyTuple", ['a', 'b', 'c'])

    print(MyView)
    print(MyTuple)

    print("[ Simple List ]")
    value = [1, 2, 3]
    view = MyView(value)
    print(MyTuple._fields)
    print(MyView.get_fields())
    print(view.get_fields())
    print(view.a)
    view[0] = 10
    print(view.a)
    view[1] = -100
    view.c = 1000

    tup = MyTuple(a=1, b=2, c=3)
    print(tup)

    print(view)
    print(value)
    view[:] = [111, 222, 333]
    print(view)
    print(value)
    try:
        view.bad_value = 1
        assert False
    except AttributeError:
        pass

    print("[ Array 1D ]")
    array = np.array([4, 5, 6])
    aview = MyView(array)
    print(aview.a)
    aview[[1, 2]] = [50, 60]
    print(array)
    print(aview)

    # Maybe not useful, but meh.
    print("[ Array 2D ]")
    mat = np.eye(3)
    mview = MyView(mat)
    print(mview)
    print(mview.a)
    print(mview[0, 0])
    mview.a[0] = 10
    print(mview.b)
    print(mat)


"""
Example output:

<class '__main__.MyView'>
<class '__main__.MyTuple'>
[ Simple List ]
('a', 'b', 'c')
('a', 'b', 'c')
('a', 'b', 'c')
1
10
MyTuple(a=1, b=2, c=3)
MyView(a=10, b=-100, c=1000)
[10, -100, 1000]
MyView(a=111, b=222, c=333)
[111, 222, 333]
[ Array 1D ]
4
[ 4 50 60]
MyView(a=4, b=50, c=60)
[ Array 2D ]
MyView(a=array([ 1.,  0.,  0.]), b=array([ 0.,  1.,  0.]), c=array([ 0.,  0.,  1.]))
[ 1.  0.  0.]
1.0
[ 0.  1.  0.]
[[ 10.   0.   0.]
 [  0.   1.   0.]
 [  0.   0.   1.]]
"""


if __name__ == "__main__":
    test_main()
