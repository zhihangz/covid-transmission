{
    alphat = turbulence->nut()/Prt;
    alphat.correctBoundaryConditions();

    volScalarField alphaEff("alphaEff", turbulence->nu()/Pr + alphat);
    volScalarField DEff("DEff", turbulence->nu()/Sc + turbulence->nut()/Sct);//Deff for C diffusion

    fvScalarMatrix TEqn
    (
        fvm::ddt(T)
      + fvm::div(phi, T)
      - fvm::laplacian(alphaEff, T)
     ==
        radiation->ST(rhoCpRef, T)
      + fvOptions(T)
    );

    TEqn.relax();

    fvOptions.constrain(TEqn);

    TEqn.solve();

    radiation->correct();

    fvOptions.correct(T);

    rhok = 1.0 - beta*(T - TRef);

    // // --- Transported scalar variables

    // // uncomment this block if air circulation through HVAC is needed, better to set a dictionary and read the patch names and circulation rate from the case
    // // find patch id for inlet and outlet
    // label Iid = mesh.boundaryMesh().findPatchID("supplyVents");//HVAC inlet
    // label Oid = mesh.boundaryMesh().findPatchID("exhaust");//HVAC return

    // // assume phi all negative, add a line max(0, phi) if not true
    // scalar Oc = sum( C.boundaryFieldRef()[Oid] * mag(phi.boundaryFieldRef()[Oid]) );
    // reduce( Oc, sumOp<scalar>() );
    // //Info << "sum of c out = " << Oc << endl;

    // scalar Iphi = sum( mag(phi.boundaryFieldRef()[Iid]) );
    // reduce( Iphi, sumOp<scalar>() );
    // //Info << "sum of phi in = " << Iphi << endl;

    // C.boundaryFieldRef()[Iid] == 0.8*mag(Oc/Iphi);//80% circulation rate

    fvScalarMatrix CEqn
    (
        fvm::ddt(C)
      + fvm::div(phi, C)
      - fvm::laplacian(DEff, C)
    );

    CEqn.relax();

    CEqn.solve();
}
