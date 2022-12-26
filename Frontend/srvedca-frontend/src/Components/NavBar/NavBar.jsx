import React, { Component } from 'react'
import './NavBar.css'

import { NavLink } from 'react-router-dom'

export class NavBar extends Component {
    render() {
        return (
        <nav>
            <NavLink to='/home'>Accueil</NavLink>
            <NavLink to='/enrol'>Etudiants Enrôlés</NavLink>
            <NavLink to='/classes'>Classes</NavLink>
            <NavLink to='/courses'>Cours</NavLink>
            <NavLink to='/admin'>Administrateurs</NavLink>
            <NavLink to='/course-planning'>Planning des cours</NavLink>
            <NavLink to='/presence'>Présences</NavLink>
        </nav>
        )
    }
}

export default NavBar