#!/usr/bin/env python3
import  argparse
import  cppyy
from    cppyy import ll

# ---
preload = [
    'G4Types.hh',
    'G4String.hh',
    'G4RunManagerFactory.hh',
    'G4UImanager.hh',
    'G4UIExecutive.hh',
    'G4VisExecutive.hh',
    'eASTInitialization.hh',
    ]

libload = [
    'libG4run',
    'libG4graphics_reps',
    'libG4intercoms',
    'libG4physicslists',
    'libG4persistency',
    'east',
]

# ---
parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose",  action='store_true', help="Verbose mode")
parser.add_argument("-m", "--macrofile",type=str,            help="The optional macro file", default='')
# ---

args    = parser.parse_args()

verb    = args.verbose
macro   = args.macrofile


for pre in preload:
    if verb: print(f'''Loading header file {pre}''')
    cppyy.include(pre)

if verb: print('--- Finished loading headers ---')

for lib in libload:
    if verb: print(f'''Loading library {lib}''')
    cppyy.load_library(lib)

if verb: print('--- Finished loading libraries ---')


# --- Initialize the critical elelement to get hold of the UI:

from cppyy.gbl import G4RunManagerFactory, G4UImanager, eASTInitialization

runManager = G4RunManagerFactory.CreateRunManager()

G4UImanager.GetUIpointer = staticmethod(G4UImanager.GetUIpointer)

UImanager = G4UImanager.GetUIpointer()

# searchpath  = UImanager.GetMacroSearchPath()
# print(searchpath)

eastI = eASTInitialization()

if macro!='':
    try:
        f=open(macro)
    except:
        print(f'''Error opening macro file "{macro}", exiting...''')
        exit(-1)

    lines = [line.rstrip() for line in f]

    if verb: print(f'''Read {len(lines)} lines from the file "{macro}"''')


    for line in lines:
        if len(line)==0: continue
        if line[0]=='#': continue
        if verb: print(f'''Executing command: {line}''')
        UImanager.ApplyCommand(line)


exit(0)



# UImanager = G4UImanager.GetUIpointer()
UImanager.ApplyCommand("#")
UImanager.ApplyCommand("/control/verbose 2")

# UImanager.ApplyCommand("/run/initialize")

# visManager = G4VisExecutive("Quiet")


# -------------- ATTIC ---------------------

# cppyy.load_library('libG4run')
# cppyy.load_library('libG4graphics_reps')
# cppyy.load_library('libG4intercoms')
# cppyy.load_library('east')