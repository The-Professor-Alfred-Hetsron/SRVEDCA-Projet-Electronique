import React, {  useEffect } from 'react'
import './ProfileButton.css'
import decode from 'jwt-decode';



const  ProfileButton = () => {

  const logout = () => {
    localStorage.clear();
    window.location="login/";
  };

  useEffect(() => {
    const token = localStorage.getItem('token');

    if (token) {
      const decodedToken = decode(token);

      if (decodedToken.exp * 1000 < new Date().getTime()) logout();
    }

  }, []);

  const user = JSON.parse(localStorage.getItem('user'));
 
    return (
      <div className='profileButton-container' onClick={() => logout()} >
        <div className='profileButton-container-img'><span>{user.nom.charAt(0)}</span></div>
        <span className='profileButton-container-span'>{user.nom}</span>
      </div>
    )
}

export default ProfileButton