#!/usr/bin/env python3
import cppyy
from cppyy import ll

preload = [
    'G4Types.hh',
    'G4String.hh',
    'G4RunManagerFactory.hh',
    'G4UImanager.hh',
    'G4UIExecutive.hh',
    ]

for pre in preload:
    print(f'''Preloading {pre}''')
    cppyy.include(pre)


cppyy.load_library('libG4run')
cppyy.load_library('libG4graphics_reps')
cppyy.load_library('libG4intercoms')


from cppyy.gbl import G4RunManagerFactory, G4UImanager

runManager = G4RunManagerFactory.CreateRunManager()

G4UImanager.GetUIpointer = staticmethod(G4UImanager.GetUIpointer)

UImanager = G4UImanager.GetUIpointer()

searchpath  = UImanager.GetMacroSearchPath()

print(searchpath)

# UImanager = G4UImanager.GetUIpointer()
# visManager = G4VisExecutive("Quiet")


