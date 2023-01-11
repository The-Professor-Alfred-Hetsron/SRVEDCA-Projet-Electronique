import React, { useState, useEffect } from "react";
import "./Presences.css";
import axios from "axios";
import moment from "moment/moment";
import PresencesTable from "./PresencesTable";

const API = " http://localhost:8080/api/";

const Presences = () => {
  const [classeId, setClasseId] = useState();
  const [coursId, setCoursId] = useState();
  const [dateDebut, setDateDebut] = useState();
  const [dateFin, setDateFin] = useState(moment().format("YYYY-MM-DD"));
  const [listeClasses, setListeClasses] = useState([]);
  const [listeCours, setListeCours] = useState([]);
  const [allCours, setAllCours] = useState(false);
  const [allDates, setAllDates] = useState(false);
  const [TimeTable, setTimeTable] = useState([]);

  const getPresences = (e)=>{e.preventDefault()

    let data = { classeId: document.querySelector("#classe").value };
    if (!allCours) data.coursId = document.querySelector("#cours").value;
    if (!allDates) {
      data.dateDebut = dateDebut;
      data.dateFin = dateFin;
    }
    // console.log(data);

    axios.post(API + 'presence/', data)
      .then(function (response) {
        // console.log(response.data);
        setTimeTable(response.data) //On actualise nos données
      })
      .catch(function (error) {
        alert(error.message)
      });
    // console.log(jourSemaine, getDate(hDebut), getDate(hFin), cours)
  }

  useEffect(() => {
    axios
      .get(API + "cours/")
      .then(function(response) {
        setListeCours(response.data);
        // setCoursId(response.data[0]._id)
      })
      .catch(function(error) {
        console.log(error);
      });
  }, []);

  useEffect(() => {
    axios
      .get(API + "classe/all")
      .then(function(response) {
        setListeClasses(response.data);
        setClasseId(response.data[0]._id); //Car la première classe est le premier élément du select
      })
      .catch(function(error) {
        console.log(error);
      });
  }, []);


  return (
    <div id="presences-body">
      <div className="entete">
        <div>
          <label htmlFor="classe">Classe: </label>
          <select
            name="classe"
            id="classe"
            onChange={(e) => setClasseId(e.currentTarget.value)}
          >
            {listeClasses.map((value) => (
              <option value={value._id}>{value.nom}</option>
            ))}
          </select>
        </div>
        <div>
          <label htmlFor="cours">Cours: </label>
          {!allCours && 
          <select
          disabled={allCours}
          name="cours"
          id="cours"
          onChange={(e) => setCoursId(e.currentTarget.value)}
        >
          {listeCours
            .filter((value) => {
              return value.classe === classeId;
            })
            .map((value) => (
              <option value={value._id}>
                {value.code + " - " + value.nom}
              </option>
            ))}
        </select>}
          <span>
            <input
              type="checkbox"
              name="allcours"
              id="allcours"
              value={allCours}
              onClick={(e) => {
                setAllCours(e.currentTarget.checked);
              }}
            />
            <label htmlFor="allcours">Tous les cours</label>
          </span>
        </div>
        <div className="ligne">
          {!allDates && (
            <div className="ligne">
              <div>
                <label htmlFor="datedebut">Du: </label>
                <input
                  disabled={allDates}
                  type="date"
                  name="datedebut"
                  id="datedebut"
                  value={dateDebut}
                  onChange={(e) => setDateDebut(e.currentTarget.value)}
                />
              </div>
              <div>
                <label htmlFor="datefin">Au: </label>
                <input
                  disabled={allDates}
                  type="date"
                  name="datefin"
                  id="datefin"
                  value={dateFin}
                  onChange={(e) => setDateFin(e.currentTarget.value)}
                />
              </div>
            </div>
          )}
          <span>
            <input
              type="checkbox"
              name="alldates"
              id="alldates"
              value={allCours}
              onClick={(e) => {
                setAllDates(e.currentTarget.checked);
              }}
            />
            <label htmlFor="alldates">Toutes les dates</label>
          </span>
        </div>
        <div className="ligne">
            <button onClick={getPresences} >Rechercher</button>
        </div>
      </div>
      <div className="corps">
        <PresencesTable tableData={TimeTable} />
      </div>
    </div>
  );
};

export default Presences;
