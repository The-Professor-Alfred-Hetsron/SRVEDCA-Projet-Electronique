import React, { Component } from 'react'
import './ClassTable.css'

export class ClassTable extends Component {
  render() {
    const salle = [
        [ "id", "Nom", "Salle", "Actions" ]
    ];
    return (
        <div class="tableau">
            <header>
                <button>Valider</button>
                <button>Imprimer</button>
                <input type="search"/>
                <button>Recherche</button>
            </header>
            <table>
                <thead>
                    <tr>
                       <th>id</th>
                       <th>Noms</th>
                       <th>Salle</th>
                       <th>Actions</th>
                    </tr>    
                </thead>

                <tbody>
                    <tr>
                        <td>1</td>
                        <td>Akoa bijoumong</td>
                        <td>Pr</td>
                        <td>
                            <button>modifier</button> <br></br>
                            <button>supprimer</button>
                        </td>
                    </tr>
                    <tr>
                        <td>1</td>
                        <td>Akoa bijoumong</td>
                        <td>Pr</td>
                        <td>
                            <button>modifier</button> <br></br>
                            <button>supprimer</button>
                        </td>
                    </tr>
                    <tr>
                        <td>1</td>
                        <td>Akoa bijoumong</td>
                        <td>Pr</td>
                        <td>
                            <button>modifier</button> <br></br>
                            <button>supprimer</button>
                        </td>
                    </tr>
                    <tr>
                        <td>1</td>
                        <td>Akoa bijoumong</td>
                        <td>Pr</td>
                        <td>
                            <button>modifier</button> <br></br>
                            <button>supprimer</button>
                        </td>
                    </tr>
                    <tr>
                        <td>1</td>
                        <td>Akoa bijoumong</td>
                        <td>Pr</td>
                        <td>
                            <button>modifier</button> <br></br>
                            <button>supprimer</button>
                        </td>
                    </tr>
                </tbody>

            </table>
        </div>
    )
  }
}

export default ClassTable