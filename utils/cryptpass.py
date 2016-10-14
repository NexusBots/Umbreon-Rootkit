import crypt, sys, string, random
salty = ''.join(random.choice(string.ascii_uppercase + string.digits + string.ascii_lowercase) for _ in range(8))
print crypt.crypt(sys.argv[1], "$6$"+salty)
