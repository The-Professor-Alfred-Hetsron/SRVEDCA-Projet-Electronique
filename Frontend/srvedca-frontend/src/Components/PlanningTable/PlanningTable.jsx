import React, { Component } from 'react'
import './PlanningTable.css'

export class PlanningTable extends Component {
  render() {
    const TimeTable = [
        {
            id: 1,
            JourSemaine: "Lundi",
            HeureDebut: "8:00",
            HeureFin: "10:00",
            codeUE: "GIF1022"

        },
        {
           
            id: 2,
            JourSemaine: "Mardi",
            HeureDebut: "15:00",
            HeureFin: "17:45",
            codeUE: "GC1223"
        }
    ]

    const planningTag = TimeTable.map((TimeTable) => <tr>
    <td>{TimeTable.id}</td>
    <td>{TimeTable.JourSemaine}</td>
    <td>{TimeTable.HeureDebut}</td>
    <td>{TimeTable.HeureFin}</td>
    <td>{TimeTable.codeUE}</td>
    <td>
        <button>modifier</button> <br></br>
        <button>supprimer</button>
    </td>
</tr>)
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
                       <th>JourSemaine</th>
                       <th>HeureDebut</th>
                       <th>HeureFin</th>
                       <th>codeUE</th>
                       <th>Actions</th>
                    </tr>    
                </thead>

                <tbody>
                    {planningTag}
                </tbody>

            </table>
        </div>
    )
  }
}

export default PlanningTable