import React, { Component } from 'react'
import "./LoginForm.css"
import { NavLink } from 'react-router-dom'

export class LoginForm extends Component {
  render() {
    return (
      <form className='cover' method='post'>
        <h1>Connexion</h1>
        <div className='input-group'>
            <input type="text" class="input" placeholder=" "/>
            <label class="placeholder"> Email</label>
        </div>
        <div className='input-group'>
            <input type="password" class='input' placeholder=" "/>
            <label class="placeholder">Mot de passe</label>
        </div>
        <NavLink className="connect-login" to="/accueil" end>
            <input type="submit" className='loginButton' value="Se Connecter"/>
        </NavLink>
      </form>
    )
  }
}

export default LoginForm