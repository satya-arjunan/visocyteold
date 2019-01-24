import visocyte.simple

pxm = visocyte.simple.servermanager.ProxyManager()
proxy = pxm.NewProxy('testcase', 'None')
print(proxy)
