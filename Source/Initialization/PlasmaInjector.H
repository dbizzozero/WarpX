/* Copyright 2019 Andrew Myers, Axel Huebl, David Grote
 * Maxence Thevenet, Remi Lehe, Weiqun Zhang
 *
 *
 * This file is part of WarpX.
 *
 * License: BSD-3-Clause-LBNL
 */
#ifndef PLASMA_INJECTOR_H_
#define PLASMA_INJECTOR_H_

#include "Particles/SpeciesPhysicalProperties.H"
#include "InjectorPosition.H"
#include "InjectorDensity.H"
#include "InjectorMomentum.H"

#include "Utils/WarpXConst.H"
#include "Parser/WarpXParser.H"

#include <AMReX_Vector.H>
#include <AMReX_ParmParse.H>
#include <AMReX_Utility.H>

#ifdef WARPX_USE_OPENPMD
#   include <openPMD/openPMD.hpp>
#endif

#include <array>

///
/// The PlasmaInjector class parses and stores information about the plasma
/// type used in the particle container. This information is used to create the
/// particles on initialization and whenever the window moves.
///
class PlasmaInjector
{

public:

    PlasmaInjector ();

    PlasmaInjector (int ispecies, const std::string& name);

    // bool: whether the point (x, y, z) is inside the plasma region
    bool insideBounds (amrex::Real x, amrex::Real y, amrex::Real z) const noexcept;

    // bool: whether the region defined by lo and hi overlaps with the plasma region
    bool overlapsWith (const amrex::XDim3& lo, const amrex::XDim3& hi) const noexcept;

    int num_particles_per_cell;

    amrex::Vector<int> num_particles_per_cell_each_dim;

    // gamma * beta
    amrex::XDim3 getMomentum (amrex::Real x, amrex::Real y, amrex::Real z) const noexcept;

    amrex::Real getCharge () {return charge;}
    amrex::Real getMass () {return mass;}
    PhysicalSpecies getPhysicalSpecies() const {return physical_species;}

    bool doInjection () const noexcept { return inj_pos != NULL;}

    bool add_single_particle = false;
    amrex::Vector<amrex::ParticleReal> single_particle_pos;
    amrex::Vector<amrex::ParticleReal> single_particle_vel;
    amrex::ParticleReal single_particle_weight;

    bool gaussian_beam = false;
    amrex::Real x_m;
    amrex::Real y_m;
    amrex::Real z_m;
    amrex::Real x_rms;
    amrex::Real y_rms;
    amrex::Real z_rms;
    amrex::Real x_cut = std::numeric_limits<amrex::Real>::max();
    amrex::Real y_cut = std::numeric_limits<amrex::Real>::max();
    amrex::Real z_cut = std::numeric_limits<amrex::Real>::max();
    amrex::Real q_tot = 0.0;
    long npart;
    int do_symmetrize = 0;

    bool external_file = false; //! initialize from an openPMD file
    amrex::Real z_shift = 0.0; //! additional z offset for particle positions
#ifdef WARPX_USE_OPENPMD
    //! openPMD::Series to load from in external_file injection
    std::unique_ptr<openPMD::Series> m_openpmd_input_series;
#endif

    bool radially_weighted = true;

    std::string str_density_function;
    std::string str_momentum_function_ux;
    std::string str_momentum_function_uy;
    std::string str_momentum_function_uz;

    amrex::Real xmin, xmax;
    amrex::Real ymin, ymax;
    amrex::Real zmin, zmax;
    amrex::Real density_min = std::numeric_limits<amrex::Real>::epsilon();
    amrex::Real density_max = std::numeric_limits<amrex::Real>::max();

    InjectorPosition* getInjectorPosition ();
    InjectorDensity*  getInjectorDensity ();
    InjectorMomentum* getInjectorMomentum ();

protected:

    amrex::Real mass, charge;

    PhysicalSpecies physical_species;

    amrex::Real density;

    int species_id;
    std::string species_name;

    std::unique_ptr<InjectorPosition> inj_pos;
    std::unique_ptr<InjectorDensity > inj_rho;
    std::unique_ptr<InjectorMomentum> inj_mom;

    void parseDensity (amrex::ParmParse& pp);
    void parseMomentum (amrex::ParmParse& pp);
};

#endif
