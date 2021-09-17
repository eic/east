---
title: "eAST proposal"
layout: base
name: proposal
---


# Motivation 


Large-scale detector systems for the EIC are designed by larger
communities. The simulation efforts typically start as standalone
exercises for each detector component with various levels of maturity,
analytical calculations, simplified Monte Carlo exercises (fast
simulations), Geant4-based Monte Carlo approaches (full simulations),
and are later being extended in several different frameworks.

It is critical in the longer term to perform studies where the
information from various detector elements and also the interaction
region, support structures and other dead material is taken into
account, which is only possible in a comprehensive simulation. This is
essential to understand, e.g., the performance at the edges of the
detector system or the effect of combining different technologies for
the electro-magnetic calorimeter. For studies of the physics reach and
detection capabilities, it is important to be able to switch detector
options with varying levels of detail, combining full simulations for
some detector components with fast simulations for the rest of the
detector system. It is also critical to build a sustainable effort for
the entire time scale of the experiments with common tunes and commonly
validated results for both fast and full simulations.

However, such detailed studies are time consuming, both from people and
computing view. To ease leveraging new and rapidly evolving computing
technologies, we plan to implement a common and integrated approach for
fast and full simulations in Geant4 with a plug and play modular
approach.

---

# Requirements 

A comprehensive and centrally maintained simulation tool based on Geant4
for both fast and full simulations with a library of potential detector
options has to be developed. The initial focus on the development will
be:

-   ability to reuse existing simulation works,
-   ease of switching detector options with comparable levels of detail,
-   ease of switching between detailed and coarse detector descriptions,
-   ease of leveraging new and rapidly evolving technologies
(e.g., AI/ML) and computing hardware (e.g. heterogeneous architectures)

After validation of the Geant4 simulations, they will be simplified
(replacing some physics aspects) for fast simulations. This approach
will allow us to use everywhere the same geometry (very important to
reduce debugging and development time) and to combine full simulation
for a subset of detectors with fast simulations for the rest of the
integrated detector. Geant4 itself and various NP and HEP experiments
already provide sub-systems for fast simulations. This will reduce the
development time of the fast simulations substantially.

The detector simulation tool will be modular, and its development will
be targeted. It will provide an interface to the output of Monte Carlo
event generators but no further work on generators. The simulation of
detector effects and detector responses (digitization) will be clearly
separated. There will be a common geometry interface between the
detector simulation and reconstruction.

The timely development of a comprehensive, unified and centrally
maintained detector simulation tool for both fast and full simulations
to serve the needs of the detector collaborations or groups is no small
task. It must take place in the context of strong development teams in
the labs together with important contributors in the universities, and
go beyond legacy software to a new common effort to be successful. This
effort builds on a sustainable common project based on Geant4 and
tailored to needs of NP experiments as they exist today and will evolve
to the 2030s.

Geant4 has the capability to support both fast and full simulation,
which we want to use for large-scale detector systems. Geant4 through
its multi-threaded reengineering has already been able to support high
concurrency heterogeneous architectures, with excellent results in the
memory economizations achieved. Leveraging and evolving this capability
as heterogeneous architectures become ever more prevalent is of great
importance for data processing and analyses for large-scale detector
systems at the EIC.

---
# Work program and deliverables

## Create CAD interface to the detector simulation tool

Fast and detailed simulations need to be able to implement updates on
detector layout fast, where these updates need to be based on detailed
design drawings with parametric surfaces in 3D. This requires as crucial
*first step* compatibility with the CAD applications being used at BNL
and JLab, for easy exchange. The most flexible format for Geant4 to
import CAD geometry is GDML. STEP is the file format commonly available
to many CAD systems. Several tools exist that convert STEP files to GDML
files. We will work with experts at BNL and JLab to conduct a survey if
STEP is available for the variety of CAD applications used. These break
down into two broad categories: mesh vs. nurbs. Mesh CAD applications
represent a structure using a collection of flat polygons that emulate
curved surfaces that can be used to represent volumes. The granularity
of the polygon surfaces can be scaled up or down to create smoother or
coarser surfaces, respectively. Examples are AutoCad and Sketchup which
are excellent for expansive structural/architectural representations of
components and buildings. Nurbs models use parameters that describe the
shape and extent of a surface, the curvature of each element, and the
thickness of the material. In effect they can produce very close
approximations of real-life entities but are difficult to exchange or
import by non-nurbs systems. Examples are CREO, SolidWorks, and Siemens
NX which are used for mechanical design. GDML also supports various
shapes and extruded solids, some of them may correspond to nurbs.
Lastly, CAD files often only contain sparse information about the
composed material, often just the name of the materials. On the other
hand, detailed detector simulations require the full composition of each
material, often down to the isotope composition level. We will develop a
mechanism, e.g., through a macro file, to associate such additional
information needed to the detector simulation.

*Deliverables:
1) Development of a CAD Interface for Detector
Simulations.
2) Macro file that allows material composition information to be easily ported to detector simulations.*

## Create an initial version of the fast and full detector simulation tool

The detector simulation tool has to be capable of doing both fast and
full simulations in one application, easily configurable for each
detector component. Also, the detector simulation tool has to easily
integrate (“plug-in”) already existing standalone simulation
applications. These requirements will be fulfilled by utilizing the
“region” mechanism of Geant4. Each detector component is represented as
a region, where geometry description including different levels of
detail, physics options including fast simulation and unique physics
model configurations, and detector responses based on geometry and
physics options are taken care of. We will develop an initial version of
such a detector simulation tool.

*Deliverable: First running version of detector simulation tool.*

## Communicate with detector study groups

The existing standalone simulation applications that are to be adapted
to the new detector simulation tool have to be examined and converted to
be “plug-able” using the “region” mechanism. We will communicate with
detector study groups and work with them to drive the efforts of
integration. Through these interactions the requirements to the detector
simulation tool will be refined as needed. The detector simulation tool
may also integrate beam-test configurations if applicable. The
deliverable will be a prototype integration of an existing simulation,
the specific target to be chosen in the first month on the basis of
importance and available developer effort during the initial funded
period. Other detector components will be added by the detector
community based on the experience from the prototype.

*Deliverable: Documented prototype integration of an existing simulation.*

## Develop and deliver a common physics list 

Geant4 offers several alternative physics models. To make comparisons
over different detector configurations, one has to use a common set of
physics models that is appropriate to NP detectors. We will develop and
deliver such a physics list. We will consult with detector study groups
and advise on validations of the physics models with beam-test results.

*Deliverable: Documented common physics list.*

## Integrate with overall software efforts

We will compile the requirements of detector simulation to the software
infrastructure (e.g., common geometry and data formats) to be shared
across the whole software chain including physics event generation,
simulation, digitization, reconstruction and analysis programs.

*Deliverable: Requirements document. *

## Deliver a detector simulation tool extensible to heterogeneous architectures

<span id="_4d34og8" class="anchor"></span>In the future, simulation jobs
may run on computers with heterogeneous hardware configurations, e.g.,
with GPGPU and/or FPGA, and with cutting edge IT technologies such as
AI/ML. Enabling the effective use of such technologies requires design
and implementation choices in the detector simulation tool. We will
ensure the extensibility of the detector simulation tool through the use
of a tasking mechanism (either PTL or TBB). We will also support the
developers of detector components of the thread-safety of their code.

*Deliverable: Proven use of a tasking mechanism.*

---
# Project leader

<span id="_2s8eyo1" class="anchor"></span>This project aims to take
advantage of the proven value for detailed nuclear and particle physics
experiment simulations by the HEP-developed Geant4 software and the
growing prospects of use of advanced computing techniques. Any
large-scale detector system will benefit from the project on “*Fast and
full simulations in Geant4 for large-scale detector systems with a plug
and play modular approach”*, with the foreseen EIC detectors prime
examples. For example, EIC detector simulation development must be able
to progress rapidly to provide the EIC user community proper flexible
and forward-looking tools to meet the simulation needs of the detailed
detector design and EIC science performance studies. This requires a
common simulation framework and effort if the EIC community is to be
properly served by a unified effort drawing on pooled expertise. This
also requires simulation tools ready to take advantage of the growing
use of heterogeneous computing environments and the prospects they
offer. This work will seed a new common effort through the leadership of
an expert of unique stature and technical expertise, who has the trust
and confidence of the full community to establish a common effort, while
also being a technical expert. The *“Fast and full simulations in Geant4
for large-scale detector systems with a plug and play modular approach”*
will ensure that any work on AI/ML and running on computers with
heterogeneous hardware configurations can be directly applied.

In Dr. Makoto Asai we identify the individual who can uniquely fill this
role. Dr. Asai has been both a Geant4 project leader and deep technical
expert for over 20 years. He is well known to and respected by the EIC
community with which he has collaborated for many years on EIC Geant4
simulation needs. He is the designer and principal developer of Geant4's
capability to support both fast and full simulation, which we want to
use for the EIC. He led Geant4 through its multi-threaded reengineering
to support high concurrency heterogeneous architectures, with excellent
results in the memory economizations achieved. Leveraging and evolving
this capability as heterogeneous architectures become ever more
prevalent is of great importance for the EIC, and together with the
integrated fast simulation support opens the door to leveraging AI/ML in
the unified simulation. Dr. Asai is also an expert in the geometry and
modular detector description tools that will be the basis of unified
geometry in detector simulations. He is also an expert in the Geant4
physics models that have to be tuned for NP experiments, having presided
over their development and integration for much of the past 20 years. No
other individual has an array of attributes better tailored to the
leader we are looking for. Finally and crucially, Dr. Asai is available
for this work if we act promptly.

---
# Appendix
## Notes from the [*March 25 EIC software meeting*](https://indico.bnl.gov/event/11102/) discussion (Torre)

-   CAD work is a distinct project, \~1mo duration, it goes first both
    because Elke and Rolf need it and because it gives us a month to
    plan the simu project specifics before it launches
-   discussion about time ordering and priorities. Note that there is no
    time or priority ordering implicit in the ordering of deliverables
    in the proposal.
-   comments from several that physics list should be highest/earliest
    priority
-   test beams should be considered and included. Test beam data is
    essential to validating physics lists. The simulation software
    should support test beam configurations.
    -   comment asking whether factoring out digitization is compatible
        with physics list tuning, which can be dependent on digi
        tuning, e.g. Birk’s constant.
    -   tuning won’t happen without actual data, will rely on test beam
    -   noted that we do already have relevant test beam data already in
        hand
    -   comment that in talking about ‘tuning’ we should really be
        calling it ‘validation’
-   discussion about the extent to which (very) fast 4vector based
    simulation like eic-smear should be integrated. Would Geant4
    integration slow it down? Why carry Geant4 dependencies for what
    would otherwise have lightweight dependencies?
    -   in writing the proposal we expected that eic-smear type
        simulations would not be integrated into Geant4 (because it
        would slow them down), but would be incorporated in as
        seamlessly as possible for ease of use, e.g. uniformity of
        user interfaces, of course uniformity of MCEG interfaces
    -   Makoto commented that it is possible to integrate such very fast
        simulations in Geant4 without a performance hit.
-   comment that it should be possible for the user to “own the event
    loop”, treating the simulation software fully as an on-call
    toolkit rather than handing over control, especially for fast
    simulation
-   comment on whether we should put such emphasis on Geant4; what if a
    new simu toolkit comes along or ‘GeantV’ emerges.

    -   ‘GeantV’ as a rewrite/reengineering of Geant4 has been laid to
        rest in recent years, as a useful learning exercise that was
        successful when it worked to augment Geant4 rather than
        replace it (e.g. VecGeom came out of it). There is not a
        prospective Geant4 replacement in sight, and there is a wide
        range of Geant4-directed R&D underway, including new projects
        in utilizing GPUs, particularly for EM/photon physics.
-   comment that it is difficult to evaluate the intended scope of
    the proposal. How will it integrate with the simu activities of
    the proto-collaborations.
    -   one reason the scope is somewhat fuzzy is that the duration
        is fuzzy. The present commitment is 4 months. Clearly the
        complete program envisioned can’t be done in 4 months (the
        specific deliverables though are intended to be achievable in
        4 months). We’ll seek support for extending it if it looks
        successful after the first couple of months.
    -   the project isn’t intended to have (and can’t
        realistically have) deliverables targeting the
        proto-collaborations in 2021, ie pre detector proposals (such
        is our expectation in writing it at least). The 2021 work in
        both the project and the proto-collaborations can focus on
        2022 convergence.
        -   possible/probable exception is providing an EIC physics list
        asap, could be used this year.
-   this simulation tool software should be in self-consistent repo(s)
    in the EIC GitHub
-   should be containerized and easily distributable/usable

## Notes from the [*March 2 EIC software meeting*](https://indico.bnl.gov/event/11151/) discussion (Markus)

-   Wouter asked about the steps after the community input. We clarified
    that there will be regular updates in the weekly EIC Software
    meetings as well as regular meetings by the developers.

-   Jin asked to publish the validation setup as part of the deliverable
    (see his comment above). We will add this to the deliverable. We
    also summarized last week’s discussion on the topic, in particular
    the requirement for test-beam data and Elke’s suggestion to
    collect information from the EIC Generic Detector R&D consortia on
    tuning and validation of detector simulations in Geant4.

-   Dmitry asked how the plug and play modular approach will
    be implemented. He also asked about the user interface, in
    particular using the Geant4 macros or Python bindings.
