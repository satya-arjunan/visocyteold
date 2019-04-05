# test to make sure that we can import visocyte when running in parallel
# from a regular Python shell. Note that PYTHONPATH needs to be set
# to know where to find ParaView.

import sys
print('sys.path = %s' % sys.path)
from mpi4py import MPI


print('proc %d' % MPI.COMM_WORLD.Get_rank())

import visocyte

visocyte.options.batch = True
visocyte.options.symmetric = True

print('about to import visocyte.simple %d' % MPI.COMM_WORLD.Get_rank())
import visocyte.simple

print('finished %d' %  MPI.COMM_WORLD.Get_rank())
