const Planning = require('../../Database/Planning')

//Creer un nouveau plannig
const store = (req, res, next) => {
    let planning = Planning({
        jourSemaine : req.body.jourSemaine,
        heureDebut : req.body.heureDebut,
        heureFin : req.body.heureFin,
        cours : req.body.cours
    })
    planning.save()
    .then(response =>{
        res.json({
            message : 'Nouveau planning ajouté !'
        })
    })
    .catch(error =>{
        res.json({
            message : 'Une erreur est survenue !'
        })
    })
}


//Afficher la liste de planning
const showAll=(req, res, next) => {
    Planning.find()
    .then(response =>{
        res.json({
            response
        })
    })
    .catch(error =>{
        res.json({
            message:'Une erreur est survenue'
        })
    })
}

// Modifier un planning

const update = (req, res, next) => {
    let planningID = req.body.planningID

    let updatePlanning = {
        jourSemaine : req.body.jourSemaine,
        heureDebut : req.body.heureDebut,
        heureFin : req.body.heureFin,
        cours : req.body.cours
    }

    Cours.findByIdAndUpdate (planningID, {$set : updatePlanning})
    .then(()=> {
        res.json({
            message : 'Planning modifié avec succèss !'
        })
    })
    .catch(error => {
        res.json({
            message : 'une erreur est survenue !'
        })
    })
}

// Supprimer un planning
const destroy = (req, res, next) => {
    let planningID = req.params.planningID
    Cours.findByIdAndRemove(planningID)
    .then(() => {
        res.json({
            message : 'Suppression du planning avec succèss !'
        })
    })
    .catch(error => {
        res.json({
            message : 'Une erreur est survenue !'
        })
    })
}


// exportation des fonctions
module.exports = {
    store, showAll, update, destroy
}
   