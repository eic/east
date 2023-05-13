#!/usr/bin/env python3
import cppyy
from cppyy import ll

preload = [
    'G4Types.hh',
    'G4String.hh',
    'G4RunManagerFactory.hh',
    'G4UImanager.hh',
    'G4UIExecutive.hh',
    'eASTInitialization.hh',
    ]

for pre in preload:
    print(f'''Preloading {pre}''')
    cppyy.include(pre)


cppyy.load_library('libG4run')
cppyy.load_library('libG4graphics_reps')
cppyy.load_library('libG4intercoms')
cppyy.load_library('east')

from cppyy.gbl import G4RunManagerFactory, G4UImanager, eASTInitialization

runManager = G4RunManagerFactory.CreateRunManager()

G4UImanager.GetUIpointer = staticmethod(G4UImanager.GetUIpointer)

UImanager = G4UImanager.GetUIpointer()

searchpath  = UImanager.GetMacroSearchPath()

print(searchpath)

eastI = eASTInitialization()

# UImanager = G4UImanager.GetUIpointer()

UImanager.ApplyCommand("/control/verbose 2")

# UImanager.ApplyCommand("/run/initialize")

# visManager = G4VisExecutive("Quiet")


