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

    // Transported scalar variables
    fvScalarMatrix CEqn
    (
        fvm::ddt(C)
      + fvm::div(phi, C)
      - fvm::laplacian(DEff, C)
    );

    CEqn.relax();

    CEqn.solve();

    Nb += C*runTime.deltaTValue();
    Info << "Solving for Nb," << tab << "dt = " << runTime.deltaTValue() << endl;
}
