/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2010-2022 Danny van Dyk
 * Copyright (c) 2018 Keri Vos
 *
 * This file is part of the EOS project. EOS is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * EOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef EOS_GUARD_EOS_FORM_FACTORS_PARAMETRIC_FVDV2018_IMPL_HH
#define EOS_GUARD_EOS_FORM_FACTORS_PARAMETRIC_FVDV2018_IMPL_HH 1

#include <eos/form-factors/parametric-fvdv2018.hh>
#include <eos/utils/kinematic.hh>

namespace eos
{
    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::_calc_z(const double & t, const double & t_p, const double & t_0)
    {
        return (std::sqrt(t_p - t) - std::sqrt(t_p - t_0)) / (std::sqrt(t_p - t) + std::sqrt(t_p - t_0));
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::_z(const double & t) const
    {
        static constexpr double mB  = Process_::mB;
        static constexpr double mP1 = Process_::mP1;
        static constexpr double mP2 = Process_::mP2;

        static constexpr double t_p = power_of<2>(mB + mP1 + mP2);
        static constexpr double t_0 = 0.0;

        return _calc_z(t, t_p, t_0);
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::_zhat(const double & that) const
    {
        static constexpr double mB    = Process_::mB;
        static constexpr double mP2   = Process_::mP2;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        static constexpr double that_p = power_of<2>(mB + mP2);
        static const     double that_0 = that_p - std::sqrt(that_p * (that_p - mBst2));

        return _calc_z(that, that_p, that_0);
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::_blaschke(const double & z, const double & zh) const
    {
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        const double zBst2  = _z(mBst2);
        const double zhBst2 = _zhat(mBst2);

        double result  = 1.0;
        result *= (1.0 - z  *  zBst2) / (z  -  zBst2);
        result *= (1.0 - zh * zhBst2) / (zh - zhBst2);

        return result;
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::_blaschke_res_qhat2(const double & z) const
    {
        static constexpr double mB    = Process_::mB;
        static constexpr double mP2   = Process_::mP2;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        static constexpr double that_p = power_of<2>(mB + mP2);

        const double zBst2  = _z(mBst2);

        double result  = 4.0 * (mBst2 - that_p);
        result *= (1.0 - z  *  zBst2) / (z  -  zBst2);

        return result;
    }

    template <typename Process_>
    FvDV2018FormFactors<Process_>::FvDV2018FormFactors(const Parameters & p, const Options &) :
        // perp
        _a_Fperp_0_0(p["B->pipi::a^Fperp_0_0@FvDV2018"], *this),
        _a_Fperp_0_1(p["B->pipi::a^Fperp_0_1@FvDV2018"], *this),
        _a_Fperp_0_2(p["B->pipi::a^Fperp_0_2@FvDV2018"], *this),
        _a_Fperp_0_3(p["B->pipi::a^Fperp_0_3@FvDV2018"], *this),
        _a_Fperp_1_0(p["B->pipi::a^Fperp_1_0@FvDV2018"], *this),
        _a_Fperp_1_1(p["B->pipi::a^Fperp_1_1@FvDV2018"], *this),
        _a_Fperp_1_2(p["B->pipi::a^Fperp_1_2@FvDV2018"], *this),
        _b_Fperp_0_0(p["B->pipi::b^Fperp_0_0@FvDV2018"], *this),
        _b_Fperp_0_1(p["B->pipi::b^Fperp_0_1@FvDV2018"], *this),
        _b_Fperp_0_2(p["B->pipi::b^Fperp_0_2@FvDV2018"], *this),
        _b_Fperp_0_3(p["B->pipi::b^Fperp_0_3@FvDV2018"], *this),
        _b_Fperp_1_0(p["B->pipi::b^Fperp_1_0@FvDV2018"], *this),
        _b_Fperp_1_1(p["B->pipi::b^Fperp_1_1@FvDV2018"], *this),
        _b_Fperp_1_2(p["B->pipi::b^Fperp_1_2@FvDV2018"], *this),
        _c_Fperp_0_0(p["B->pipi::c^Fperp_0_0@FvDV2018"], *this),
        _c_Fperp_0_1(p["B->pipi::c^Fperp_0_1@FvDV2018"], *this),
        _c_Fperp_0_2(p["B->pipi::c^Fperp_0_2@FvDV2018"], *this),
        _c_Fperp_0_3(p["B->pipi::c^Fperp_0_3@FvDV2018"], *this),
        _c_Fperp_1_0(p["B->pipi::c^Fperp_1_0@FvDV2018"], *this),
        _c_Fperp_1_1(p["B->pipi::c^Fperp_1_1@FvDV2018"], *this),
        _c_Fperp_1_2(p["B->pipi::c^Fperp_1_2@FvDV2018"], *this),
        // para
        _a_Fpara_0_0(p["B->pipi::a^Fpara_0_0@FvDV2018"], *this),
        _a_Fpara_0_1(p["B->pipi::a^Fpara_0_1@FvDV2018"], *this),
        _a_Fpara_0_2(p["B->pipi::a^Fpara_0_2@FvDV2018"], *this),
        _a_Fpara_0_3(p["B->pipi::a^Fpara_0_3@FvDV2018"], *this),
        _a_Fpara_1_0(p["B->pipi::a^Fpara_1_0@FvDV2018"], *this),
        _a_Fpara_1_1(p["B->pipi::a^Fpara_1_1@FvDV2018"], *this),
        _a_Fpara_1_2(p["B->pipi::a^Fpara_1_2@FvDV2018"], *this),
        _b_Fpara_0_0(p["B->pipi::b^Fpara_0_0@FvDV2018"], *this),
        _b_Fpara_0_1(p["B->pipi::b^Fpara_0_1@FvDV2018"], *this),
        _b_Fpara_0_2(p["B->pipi::b^Fpara_0_2@FvDV2018"], *this),
        _b_Fpara_0_3(p["B->pipi::b^Fpara_0_3@FvDV2018"], *this),
        _b_Fpara_1_0(p["B->pipi::b^Fpara_1_0@FvDV2018"], *this),
        _b_Fpara_1_1(p["B->pipi::b^Fpara_1_1@FvDV2018"], *this),
        _b_Fpara_1_2(p["B->pipi::b^Fpara_1_2@FvDV2018"], *this),
        _c_Fpara_0_0(p["B->pipi::c^Fpara_0_0@FvDV2018"], *this),
        _c_Fpara_0_1(p["B->pipi::c^Fpara_0_1@FvDV2018"], *this),
        _c_Fpara_0_2(p["B->pipi::c^Fpara_0_2@FvDV2018"], *this),
        _c_Fpara_0_3(p["B->pipi::c^Fpara_0_3@FvDV2018"], *this),
        _c_Fpara_1_0(p["B->pipi::c^Fpara_1_0@FvDV2018"], *this),
        _c_Fpara_1_1(p["B->pipi::c^Fpara_1_1@FvDV2018"], *this),
        _c_Fpara_1_2(p["B->pipi::c^Fpara_1_2@FvDV2018"], *this),
        // long
        _a_Flong_0_0(p["B->pipi::a^Flong_0_0@FvDV2018"], *this),
        _a_Flong_0_1(p["B->pipi::a^Flong_0_1@FvDV2018"], *this),
        _a_Flong_0_2(p["B->pipi::a^Flong_0_2@FvDV2018"], *this),
        _a_Flong_0_3(p["B->pipi::a^Flong_0_3@FvDV2018"], *this),
        _a_Flong_1_0(p["B->pipi::a^Flong_1_0@FvDV2018"], *this),
        _a_Flong_1_1(p["B->pipi::a^Flong_1_1@FvDV2018"], *this),
        _a_Flong_1_2(p["B->pipi::a^Flong_1_2@FvDV2018"], *this),
        _b_Flong_0_0(p["B->pipi::b^Flong_0_0@FvDV2018"], *this),
        _b_Flong_0_1(p["B->pipi::b^Flong_0_1@FvDV2018"], *this),
        _b_Flong_0_2(p["B->pipi::b^Flong_0_2@FvDV2018"], *this),
        _b_Flong_0_3(p["B->pipi::b^Flong_0_3@FvDV2018"], *this),
        _b_Flong_1_0(p["B->pipi::b^Flong_1_0@FvDV2018"], *this),
        _b_Flong_1_1(p["B->pipi::b^Flong_1_1@FvDV2018"], *this),
        _b_Flong_1_2(p["B->pipi::b^Flong_1_2@FvDV2018"], *this),
        _c_Flong_0_0(p["B->pipi::c^Flong_0_0@FvDV2018"], *this),
        _c_Flong_0_1(p["B->pipi::c^Flong_0_1@FvDV2018"], *this),
        _c_Flong_0_2(p["B->pipi::c^Flong_0_2@FvDV2018"], *this),
        _c_Flong_0_3(p["B->pipi::c^Flong_0_3@FvDV2018"], *this),
        _c_Flong_1_0(p["B->pipi::c^Flong_1_0@FvDV2018"], *this),
        _c_Flong_1_1(p["B->pipi::c^Flong_1_1@FvDV2018"], *this),
        _c_Flong_1_2(p["B->pipi::c^Flong_1_2@FvDV2018"], *this),
        // time
        _a_Ftime_0_0(p["B->pipi::a^Ftime_0_0@FvDV2018"], *this),
        _a_Ftime_0_1(p["B->pipi::a^Ftime_0_1@FvDV2018"], *this),
        _a_Ftime_0_2(p["B->pipi::a^Ftime_0_2@FvDV2018"], *this),
        _a_Ftime_0_3(p["B->pipi::a^Ftime_0_3@FvDV2018"], *this),
        _a_Ftime_1_0(p["B->pipi::a^Ftime_1_0@FvDV2018"], *this),
        _a_Ftime_1_1(p["B->pipi::a^Ftime_1_1@FvDV2018"], *this),
        _a_Ftime_1_2(p["B->pipi::a^Ftime_1_2@FvDV2018"], *this),
        _b_Ftime_0_0(p["B->pipi::b^Ftime_0_0@FvDV2018"], *this),
        _b_Ftime_0_1(p["B->pipi::b^Ftime_0_1@FvDV2018"], *this),
        _b_Ftime_0_2(p["B->pipi::b^Ftime_0_2@FvDV2018"], *this),
        _b_Ftime_0_3(p["B->pipi::b^Ftime_0_3@FvDV2018"], *this),
        _b_Ftime_1_0(p["B->pipi::b^Ftime_1_0@FvDV2018"], *this),
        _b_Ftime_1_1(p["B->pipi::b^Ftime_1_1@FvDV2018"], *this),
        _b_Ftime_1_2(p["B->pipi::b^Ftime_1_2@FvDV2018"], *this),
        _c_Ftime_0_0(p["B->pipi::c^Ftime_0_0@FvDV2018"], *this),
        _c_Ftime_0_1(p["B->pipi::c^Ftime_0_1@FvDV2018"], *this),
        _c_Ftime_0_2(p["B->pipi::c^Ftime_0_2@FvDV2018"], *this),
        _c_Ftime_0_3(p["B->pipi::c^Ftime_0_3@FvDV2018"], *this),
        _c_Ftime_1_0(p["B->pipi::c^Ftime_1_0@FvDV2018"], *this),
        _c_Ftime_1_1(p["B->pipi::c^Ftime_1_1@FvDV2018"], *this),
        _c_Ftime_1_2(p["B->pipi::c^Ftime_1_2@FvDV2018"], *this)
    {
    }

    template <typename Process_>
    FvDV2018FormFactors<Process_>::~FvDV2018FormFactors() = default;

    template <typename Process_>
    FormFactors<PToPP> *
    FvDV2018FormFactors<Process_>::make(const Parameters & parameters, const Options & options)
    {
        return new FvDV2018FormFactors(parameters, options);
    }

    template <typename Process_>
    complex<double>
    FvDV2018FormFactors<Process_>::f_perp(const double & q2, const double & k2, const double & ctheta) const
    {
        static constexpr double mB  = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mP2 = Process_::mP2, mP22 = mP2 * mP2;

        const double lambda = eos::lambda(q2, k2, mB2);
        const double E2     = (mB2 + k2 - q2 - ctheta * std::sqrt(lambda)) / (4.0 * mB);
        const double qhat2  = mB2 + mP22 - 2.0 * mB * E2;

        const double z  = this->_z(q2);
        const double zh = this->_zhat(qhat2);

        const double a = _a_Fperp_0_0 + _a_Fperp_1_0 * z + _a_Fperp_0_1 * zh + _a_Fperp_1_1 * z * zh + _a_Fperp_1_2 * z * zh * zh + _a_Fperp_0_2 * zh * zh + _a_Fperp_0_3 * zh * zh * zh;
        const double b = _b_Fperp_0_0 + _b_Fperp_1_0 * z + _b_Fperp_0_1 * zh + _b_Fperp_1_1 * z * zh + _b_Fperp_1_2 * z * zh * zh + _b_Fperp_0_2 * zh * zh + _b_Fperp_0_3 * zh * zh * zh;
        const double c = _c_Fperp_0_0 + _c_Fperp_1_0 * z + _c_Fperp_0_1 * zh + _c_Fperp_1_1 * z * zh + _c_Fperp_1_2 * z * zh * zh + _c_Fperp_0_2 * zh * zh + _c_Fperp_0_3 * zh * zh * zh;

        const double blaschke = this->_blaschke(z, zh);

        complex<double> result{ 0.0, blaschke * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * std::sqrt(lambda) / (mB * std::sqrt(k2)) };

        return result;
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::f_perp_im_res_qhat2(const double & q2, const double & k2) const
    {
        static constexpr double mB    = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        const double lambda = eos::lambda(q2, k2, mB2);
        const double z  = this->_z(q2);
        const double zh = this->_z(mBst2);

        const double a = _a_Fperp_0_0 + _a_Fperp_1_0 * z + _a_Fperp_0_1 * zh + _a_Fperp_1_1 * z * zh + _a_Fperp_1_2 * z * zh * zh + _a_Fperp_0_2 * zh * zh + _a_Fperp_0_3 * zh * zh * zh;
        const double b = _b_Fperp_0_0 + _b_Fperp_1_0 * z + _b_Fperp_0_1 * zh + _b_Fperp_1_1 * z * zh + _b_Fperp_1_2 * z * zh * zh + _b_Fperp_0_2 * zh * zh + _b_Fperp_0_3 * zh * zh * zh;
        const double c = _c_Fperp_0_0 + _c_Fperp_1_0 * z + _c_Fperp_0_1 * zh + _c_Fperp_1_1 * z * zh + _c_Fperp_1_2 * z * zh * zh + _c_Fperp_0_2 * zh * zh + _c_Fperp_0_3 * zh * zh * zh;

        const double blaschke_res_qhat2 = this->_blaschke_res_qhat2(z);

        double result = blaschke_res_qhat2 * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * std::sqrt(lambda) / (mB * std::sqrt(k2));

        return result;
    }

    template <typename Process_>
    complex<double>
    FvDV2018FormFactors<Process_>::f_para(const double & q2, const double & k2, const double & ctheta) const
    {
        static constexpr double mB  = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mP2 = Process_::mP2, mP22 = mP2 * mP2;

        const double lambda = eos::lambda(q2, k2, mB2);
        const double E2     = (mB2 + k2 - q2 - ctheta * std::sqrt(lambda)) / (4.0 * mB);
        const double qhat2  = mB2 + mP22 - 2.0 * mB * E2;

        const double z  = this->_z(q2);
        const double zh = this->_zhat(qhat2);

        const double a = _a_Fpara_0_0 + _a_Fpara_1_0 * z + _a_Fpara_0_1 * zh + _a_Fpara_1_1 * z * zh + _a_Fpara_1_2 * z * zh * zh + _a_Fpara_0_2 * zh * zh + _a_Fpara_0_3 * zh * zh * zh;
        const double b = _b_Fpara_0_0 + _b_Fpara_1_0 * z + _b_Fpara_0_1 * zh + _b_Fpara_1_1 * z * zh + _b_Fpara_1_2 * z * zh * zh + _b_Fpara_0_2 * zh * zh + _b_Fpara_0_3 * zh * zh * zh;
        const double c = _c_Fpara_0_0 + _c_Fpara_1_0 * z + _c_Fpara_0_1 * zh + _c_Fpara_1_1 * z * zh + _c_Fpara_1_2 * z * zh * zh + _c_Fpara_0_2 * zh * zh + _c_Fpara_0_3 * zh * zh * zh;

        const double blaschke = this->_blaschke(z, zh);

        complex<double> result{ 0.0, blaschke * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB / std::sqrt(k2) };

        return result;
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::f_para_im_res_qhat2(const double & q2, const double & k2) const
    {
        static constexpr double mB    = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        const double z  = this->_z(q2);
        const double zh = this->_z(mBst2);

        const double a = _a_Fpara_0_0 + _a_Fpara_1_0 * z + _a_Fpara_0_1 * zh + _a_Fpara_1_1 * z * zh + _a_Fpara_1_2 * z * zh * zh + _a_Fpara_0_2 * zh * zh + _a_Fpara_0_3 * zh * zh * zh;
        const double b = _b_Fpara_0_0 + _b_Fpara_1_0 * z + _b_Fpara_0_1 * zh + _b_Fpara_1_1 * z * zh + _b_Fpara_1_2 * z * zh * zh + _b_Fpara_0_2 * zh * zh + _b_Fpara_0_3 * zh * zh * zh;
        const double c = _c_Fpara_0_0 + _c_Fpara_1_0 * z + _c_Fpara_0_1 * zh + _c_Fpara_1_1 * z * zh + _c_Fpara_1_2 * z * zh * zh + _c_Fpara_0_2 * zh * zh + _c_Fpara_0_3 * zh * zh * zh;

        const double blaschke_res_qhat2 = this->_blaschke_res_qhat2(z);

        double result = blaschke_res_qhat2 * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB / std::sqrt(k2);

        return result;
    }

    template <typename Process_>
    complex<double>
    FvDV2018FormFactors<Process_>::f_long(const double & q2, const double & k2, const double & ctheta) const
    {
        static constexpr double mB  = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mP2 = Process_::mP2, mP22 = mP2 * mP2;

        const double lambda = eos::lambda(q2, k2, mB2);
        const double E2     = (mB2 + k2 - q2 - ctheta * std::sqrt(lambda)) / (4.0 * mB);
        const double qhat2  = mB2 + mP22 - 2.0 * mB * E2;

        const double z  = this->_z(q2);
        const double zh = this->_zhat(qhat2);

        const double a = _a_Flong_0_0 + _a_Flong_1_0 * z + _a_Flong_0_1 * zh + _a_Flong_1_1 * z * zh + _a_Flong_1_2 * z * zh * zh + _a_Flong_0_2 * zh * zh + _a_Flong_0_3 * zh * zh * zh;
        const double b = _b_Flong_0_0 + _b_Flong_1_0 * z + _b_Flong_0_1 * zh + _b_Flong_1_1 * z * zh + _b_Flong_1_2 * z * zh * zh + _b_Flong_0_2 * zh * zh + _b_Flong_0_3 * zh * zh * zh;
        const double c = _c_Flong_0_0 + _c_Flong_1_0 * z + _c_Flong_0_1 * zh + _c_Flong_1_1 * z * zh + _c_Flong_1_2 * z * zh * zh + _c_Flong_0_2 * zh * zh + _c_Flong_0_3 * zh * zh * zh;

        const double blaschke = this->_blaschke(z, zh);

        complex<double> result{ 0.0, blaschke * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB / std::sqrt(q2) * mB2 / std::sqrt(lambda) * mB2 / k2 };

        return result;
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::f_long_im_res_qhat2(const double & q2, const double & k2) const
    {
        static constexpr double mB    = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        const double lambda = eos::lambda(q2, k2, mB2);
        const double z  = this->_z(q2);
        const double zh = this->_z(mBst2);

        const double a = _a_Flong_0_0 + _a_Flong_1_0 * z + _a_Flong_0_1 * zh + _a_Flong_1_1 * z * zh + _a_Flong_1_2 * z * zh * zh + _a_Flong_0_2 * zh * zh + _a_Flong_0_3 * zh * zh * zh;
        const double b = _b_Flong_0_0 + _b_Flong_1_0 * z + _b_Flong_0_1 * zh + _b_Flong_1_1 * z * zh + _b_Flong_1_2 * z * zh * zh + _b_Flong_0_2 * zh * zh + _b_Flong_0_3 * zh * zh * zh;
        const double c = _c_Flong_0_0 + _c_Flong_1_0 * z + _c_Flong_0_1 * zh + _c_Flong_1_1 * z * zh + _c_Flong_1_2 * z * zh * zh + _c_Flong_0_2 * zh * zh + _c_Flong_0_3 * zh * zh * zh;

        const double blaschke_res_qhat2 = this->_blaschke_res_qhat2(z);

        double result = blaschke_res_qhat2 * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB / std::sqrt(q2) * mB2 / std::sqrt(lambda) * mB2 / k2;

        return result;
    }

    template <typename Process_>
    complex<double>
    FvDV2018FormFactors<Process_>::f_time(const double & q2, const double & k2, const double & ctheta) const
    {
        static constexpr double mB  = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mP2 = Process_::mP2, mP22 = mP2 * mP2;

        const double lambda = eos::lambda(q2, k2, mB2);
        const double E2     = (mB2 + k2 - q2 - ctheta * std::sqrt(lambda)) / (4.0 * mB);
        const double qhat2  = mB2 + mP22 - 2.0 * mB * E2;

        const double z  = this->_z(q2);
        const double zh = this->_zhat(qhat2);

        const double a = _a_Ftime_0_0 + _a_Ftime_1_0 * z + _a_Ftime_0_1 * zh + _a_Ftime_1_1 * z * zh + _a_Ftime_1_2 * z * zh * zh + _a_Ftime_0_2 * zh * zh + _a_Ftime_0_3 * zh * zh * zh;
        const double b = _b_Ftime_0_0 + _b_Ftime_1_0 * z + _b_Ftime_0_1 * zh + _b_Ftime_1_1 * z * zh + _b_Ftime_1_2 * z * zh * zh + _b_Ftime_0_2 * zh * zh + _b_Ftime_0_3 * zh * zh * zh;
        const double c = _c_Ftime_0_0 + _c_Ftime_1_0 * z + _c_Ftime_0_1 * zh + _c_Ftime_1_1 * z * zh + _c_Ftime_1_2 * z * zh * zh + _c_Ftime_0_2 * zh * zh + _c_Ftime_0_3 * zh * zh * zh;

        const double blaschke = this->_blaschke(z, zh);

        complex<double> result{ 0.0, blaschke * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB * mB2 / std::sqrt(q2) / k2 };

        return result;
    }

    template <typename Process_>
    double
    FvDV2018FormFactors<Process_>::f_time_im_res_qhat2(const double & q2, const double & k2) const
    {
        static constexpr double mB    = Process_::mB,  mB2  = mB  * mB;
        static constexpr double mBst2 = power_of<2>(Process_::mBst);

        const double z  = this->_z(q2);
        const double zh = this->_z(mBst2);

        const double a = _a_Ftime_0_0 + _a_Ftime_1_0 * z + _a_Ftime_0_1 * zh + _a_Ftime_1_1 * z * zh + _a_Ftime_1_2 * z * zh * zh + _a_Ftime_0_2 * zh * zh + _a_Ftime_0_3 * zh * zh * zh;
        const double b = _b_Ftime_0_0 + _b_Ftime_1_0 * z + _b_Ftime_0_1 * zh + _b_Ftime_1_1 * z * zh + _b_Ftime_1_2 * z * zh * zh + _b_Ftime_0_2 * zh * zh + _b_Ftime_0_3 * zh * zh * zh;
        const double c = _c_Ftime_0_0 + _c_Ftime_1_0 * z + _c_Ftime_0_1 * zh + _c_Ftime_1_1 * z * zh + _c_Ftime_1_2 * z * zh * zh + _c_Ftime_0_2 * zh * zh + _c_Ftime_0_3 * zh * zh * zh;

        const double blaschke_res_qhat2 = this->_blaschke_res_qhat2(z);

        double result = blaschke_res_qhat2 * (a + b * (mB2 - k2) / mB2 + c * pow((mB2 - k2) / mB2, 2)) * mB * mB2 / std::sqrt(q2) / k2;

        return result;
    }
}

#endif