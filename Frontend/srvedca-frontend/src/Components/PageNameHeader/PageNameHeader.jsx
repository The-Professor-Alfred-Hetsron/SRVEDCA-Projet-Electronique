import React, { Component } from 'react'
import "./PageNameHeader.css"


export class PageNameHeader extends Component {
  render() {
    return (
      <div className='pageNameHeader-container'>
        <span>Accueil</span>
        <span>SYSTEME DE RECONNAISSANCE VOCALE ET D’EMPREINTE DIGITALE POUR LE CONTROLE D’ACCES (SRVEDCA)</span>
      </div>
    )
  }
}

export default PageNameHeader