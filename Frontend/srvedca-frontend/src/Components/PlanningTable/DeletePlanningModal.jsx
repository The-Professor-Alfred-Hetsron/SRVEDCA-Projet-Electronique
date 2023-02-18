import React from "react";
import Modal from "react-modal";
import axios from 'axios'
import './CreatePlanningModal.css'
import { useState } from "react";
import moment from "moment/moment";
import { useEffect } from "react";

const customStyles = {
  content: {
    top: "50%",
    left: "50%",
    right: "auto",
    bottom: "auto",
    marginRight: "-50%",
    transform: "translate(-50%, -50%)",
  },
};

function getDate(timeString){
    let tab = timeString.split(':')
    return moment({hour: Number(tab[0]), minute: Number(tab[1])}).toDate()
}
const jours = ["Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"]
const API = " http://localhost:8080/api/"


// Make sure to bind modal to your appElement (https://reactcommunity.org/react-modal/accessibility/)
Modal.setAppElement("#root");

const DeletePlanningModal = ({
  IsOpen,
  afterOpen,
  close, updateTable, data
}) => {
  
    const [requestOK, setRequestOK] = useState("") //Le résultat de la requête en cas de réussite
    const [requestFail, setRequestFail] = useState("") //Le résultat de la requête en cas d'échec
    const [jourSemaine, setJourSemaine] = useState(data.jourSemaine)
    const [hFin, setHFin] = useState(data.hFin)
    const [hDebut, setHDebut] = useState(data.hDebut)
    const [cours, setCours] = useState(data.cours._id)
    const [listeCours, setListeCours] = useState([])
    
    useEffect(()=>{
        axios.get(API + 'cours/all')
      .then(function (response) {
        setListeCours(response.data)
      })
      .catch(function (error) {
        console.log(error);
      })
    }, [])

    const deletePlanning = (e)=>{e.preventDefault()
        //console.log(data)
        axios.delete(API + 'planning/delete/' + data._id)
          .then(function (response) {
            // console.log(response);
            updateTable() //On actualise nos données
            close() //On ferme la boîte de dialogue
            setRequestFail('')
          })
          .catch(function (error) {
            setRequestFail(error.message)
          });
        // console.log(jourSemaine, getDate(hDebut), getDate(hFin), cours)
      }

  return (
    <div>
      <Modal
        isOpen={IsOpen}
        onAfterOpen={afterOpen}
        onRequestClose={close}
        style={customStyles}
        contentLabel="Example Modal"
      >
        <h2 className="red">Voulez-vous vraiment supprimer cet élément?</h2>
        
        <form className="createModal">
          <div>
            <label htmlFor="joursemaine">Jour</label>
            <select name="joursemaine" id="joursemaine" value={jourSemaine} onChange={(e)=>setJourSemaine(e.currentTarget.value)} disabled>
                {jours.map((value, index)=>
                    <option key={index} value={index+1}>{value}</option>
                )}
            </select>
          </div>
          <div>
            <label htmlFor="heuredebut">Heure début</label>
            <input type="time" name="heuredebut" id="heuredebut" value={hDebut} onChange={(e)=>setHDebut(e.currentTarget.value)} disabled/>
          </div>
          <div>
            <label htmlFor="heurefin">Heure fin</label>
            <input type="time" name="heurefin" id="heurefin" value={hFin} onChange={(e)=>setHFin(e.currentTarget.value)} disabled/>
          </div>
          <div>
            <label htmlFor="cours">Cours</label>
            <select name="cours" id="cours" value={cours} onChange={(e)=>setCours(e.currentTarget.value)} disabled>
                {listeCours.map((value, index)=>
                    <option key={index} value={value._id}>{value.code + ' - ' + value.nom}</option>
                )}
            </select>
          </div>
          <div className="ok">
            {requestOK}
          </div>
          <div className="fail">
            {requestFail}
          </div>
          <div>
            <button onClick={deletePlanning} >Supprimer</button>
            <button onClick={close}>Fermer</button>
          </div>
        </form>
      </Modal>
    </div>
  );

};

export default DeletePlanningModal;
