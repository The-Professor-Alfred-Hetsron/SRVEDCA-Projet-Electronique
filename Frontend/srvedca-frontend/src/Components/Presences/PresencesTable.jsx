import React from 'react';

const PresencesTable = ({tableData, infoData}) => {

    const presencesTag = tableData
    .map((data, index) => <tr>
    <td>{index + 1}</td>
    <td>{data.etudiant.nom}</td>
    <td>{data.cours.code} <br/> {data.cours.nom}</td>
    <td>{data.date}</td>
    <td>{data.heure}</td>    
</tr>)

    return (
        <div className='tableau'>
        <div class="table-header">
        <header>
            <button onClick={printContent} >Imprimer</button>
            {/* <input type="search" value={searchText} onChange = {(e)=>setSearchText(e.currentTarget.value)} />
            <button>Recherche</button> */}
        </header>
        </div>
        <table>
            <thead>
                <tr>
                   <th>id</th>
                   <th>Etudiant</th>
                   <th>Cours</th>
                   <th>Date</th>
                   <th>Heure</th>
                </tr>    
            </thead>

            <tbody>
                {presencesTag}
            </tbody>

        </table>
        {tableData.length===0 && <h3>Aucun résultat!</h3>}
        </div>
    );

    function printContent() {
    
        const toPrint = document.querySelector('table').outerHTML
        // let element = <div><h2>Liste des présences, classe de {infoData.classe}</h2>
        // <span>Cours de: {infoData.cours}</span></div>
    
        let a = window.open('', '', 'height=650, width=900');
        let head = document.querySelector("head").outerHTML
        a.document.write('<html>');
        a.document.write(head);
        a.document.write('<body>');
        // a.document.write('<h2>Liste des présences, classe de ' + infoData.classe + '</h2>');
        a.document.write(toPrint);
        a.document.write('</body></html>');
        a.document.close();
        a.print();
    }
};

export default PresencesTable;