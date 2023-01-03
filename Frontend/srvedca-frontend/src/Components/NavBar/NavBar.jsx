import React, { Component } from 'react'
import './NavBar.css'

import {LinkLayout} from '../../Components'
//import { NavLink } from 'react-router-dom'

export class NavBar extends Component {
    render() {
        return(
            <nav className="navigation-container">
                <LinkLayout/>
                <LinkLayout/>
                <LinkLayout/>
                <LinkLayout/>
                <LinkLayout/>
                <LinkLayout/>
                <LinkLayout/>

                {/*<nav>
            <NavLink to='/home'>Accueil</NavLink>
            <NavLink to='/etudiant'>Etudiants Enrôlés</NavLink>
            <NavLink to='/classe'>Classes</NavLink>
            <NavLink to='/cours'>Cours</NavLink>
            <NavLink to='/admin'>Administrateurs</NavLink>
            <NavLink to='/planning'>Planning des cours</NavLink>
        <NavLink to='/presence'>Présences</NavLink>
        </nav>*/}
            </nav>
            
        )
    }
}

export default NavBar