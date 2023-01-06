import React, { Component } from 'react'
import './MainLeftSection.css'

import {MainWelcomeHeader} from '../../Components'
import {HomeControls} from '../../Components'
import {ClassTable} from '../../Components'

export class MainLeftSection extends Component {
  render() {
    const pageType = this.props.page
    console.log(pageType)
    return (
      <div className='mainLeftSection-container'>
        {pageType === "accueil" ? /*La page Accueil du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            <MainWelcomeHeader/>
            <HomeControls/>
          </div>
        ) 
        : null}
        {pageType === "etudiant" ?  /*La page Etudiant Enrolés du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Etudiant Enrolés ici
          </div>
        ) 
        : null}
        {pageType === "classe" ?  /*La page Classes du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <ClassTable/>
          </div>
        ) 
        : null}
        {pageType === "cours" ?  /*La page Cours du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Cours ici
          </div>
        ) 
        : null}
        {pageType === "admin" ?  /*La page Administrateur du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Administrateur ici
          </div>
        ) 
        : null}
        {pageType === "planning" ?  /*La page Planning de Cours du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Planning de Cours ici
          </div>
        ) 
        : null}
        {pageType === "présence" ?  /*La page Présences du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Présences ici
          </div>
        ) 
        : null}
      </div>
    )
  }
}

export default MainLeftSection