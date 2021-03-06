/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Released 2008-2011 OpenCFD Ltd.
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    buoyantBoussinesqPimpleFoam

Group
    grpHeatTransferSolvers

Description
    Transient solver for buoyant, turbulent flow of incompressible fluids.

    Uses the Boussinesq approximation:
    \f[
        rho_{k} = 1 - beta(T - T_{ref})
    \f]

    where:
        \f$ rho_{k} \f$ = the effective (driving) kinematic density
        beta = thermal expansion coefficient [1/K]
        T = temperature [K]
        \f$ T_{ref} \f$ = reference temperature [K]

    Valid when:
    \f[
        \frac{beta(T - T_{ref})}{rho_{ref}} << 1
    \f]

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "radiationModel.H"
#include "fvOptions.H"
#include "pimpleControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for buoyant, turbulent flow"
        " of incompressible fluids.\n"
        "Uses the Boussinesq approximation."
    );

    #include "postProcess.H"

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"
    #include "createTimeControls.H"
    #include "CourantNo.H"
    #include "setInitialDeltaT.H"
    #include "initContinuityErrs.H"

    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #include "OFstream.H"
    fileName fname = ("total-C");
    OFstream os(fname);
    os << "Time" << tab << "total-C" << tab << "std-C" << endl;

    const scalarField& vol = mesh.V();
    scalar ttvol = gSum(vol);
    Info << "total volume = " << ttvol << endl;
    const dimensionedScalar one{"one",C.dimensions(),1.0};

    // switch of body force, 0 is open, 1 is closed, see UEqn
    scalar swtch = 0.0;
    scalar t;

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"

        ++runTime;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        // door switch, routine of Central 2 Outbound
        t = runTime.value();
        if (t<120.0)
            swtch = 1.0;
        else if (t<150.0)
            swtch = 0.0;

        else if (t<258.0)
            swtch = 1.0;
        else if (t<288.0)
            swtch = 0.0;

        else if (t<426.0)
            swtch = 1.0;
        else if (t<456.0)
            swtch = 0.0;

        else if (t<498.0)
            swtch = 1.0;
        else if (t<528.0)
            swtch = 0.0;

        else if (t<648.0)
            swtch = 1.0;
        else if (t<678.0)
            swtch = 0.0;

        else if (t<756.0)
            swtch = 1.0;
        else if (t<786.0)
            swtch = 0.0;
        else
            Info << "Not within switch range!!!" <<endl;

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"
            #include "TEqn.H" 

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }

            if (pimple.turbCorr())
            {
                laminarTransport.correct();
                turbulence->correct();
            }
        }

        runTime.write();

        runTime.printExecutionTime(Info);

        scalar tC = fvc::domainIntegrate(C).value();
        Info << "total C = " << tC << endl;
        scalarField dC(C/one-tC/ttvol);
        scalar std = Foam::sqrt(gSum(vol*dC*dC)/ttvol);
        Info << "std of C = " << std << endl;
        os << runTime.timeName() << tab << tC << tab << std << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
