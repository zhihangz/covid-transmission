singlePhaseTransportModel laminarTransport(U, phi);

// Thermal expansion coefficient [1/K]
dimensionedScalar beta
(
    "beta",
    dimless/dimTemperature,
    laminarTransport
);

// Reference temperature [K]
dimensionedScalar TRef("TRef", dimTemperature, laminarTransport);

// Laminar Prandtl number and Schmidt number
dimensionedScalar Pr("Pr", dimless, laminarTransport);
dimensionedScalar Sc("Sc", dimless, laminarTransport);

// Turbulent Prandtl number and Schmidt number
dimensionedScalar Prt("Prt", dimless, laminarTransport);
dimensionedScalar Sct("Sct", dimless, laminarTransport);
