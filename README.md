# cpp-vtest

C++ version of **vtest**. The Python version is [py-vtest](https://github.com/zhangtianli2006/py-vtest).

## Warning

This version does not support the `<data path>` config in `vtest.conf`.

So `vtest.conf` should look like this:

```plaintext
<name> <subtask count>
<case count for subtask #1>
<case count for subtask #2>
...
<case count for subtask #n>
```
