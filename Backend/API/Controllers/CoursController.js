const Cours = require('../../Database/Cours')

// Creer un nouveau cours
const store = (req, res, next) => {
     let cours = new Cours({
        code : req.body.code,
        nom :  req.body.nom,
        classe : req.body.classe,
        semestre : req.body.semestre
     })
     cours.save()
     .then(response =>{
        res.json({
            message : 'Nouveau Cour ajouté !'
        })
    })
    .catch(error =>{
        res.json({
            message : 'Une erreur est survenue !'
        })
    })
}

//Montrer la liste de cours
const showAll=(req, res, next) => {
    Cours.find()
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

//Afficher un cour par son nom

const showByName=(req, res, next) => {
    let coursName = req.body.nom
    Cours.findOne({$where : {nom : coursName}})
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

//Modifier un cours
const update = (req, res, next) => {
    let coursCode = req.body.code

    let updateCours = {
        code : req.body.code,
        nom :  req.body.nom,
        classe : req.body.classe,
        semestre : req.body.semestre
    }

    Cours.findByIdAndUpdate (coursCode, {$set : updateCours})
    .then(()=> {
        res.json({
            message : 'Cours modifié avec succèss !'
        })
    })
    .catch(error => {
        res.json({
            message : 'une erreur est survenue !'
        })
    })
}

// Supprimer un cours
const destroy = (req, res, next) => {
    let coursCode = req.body.code
    Cours.findByIdAndRemove(coursCode)
    .then(() => {
        res.json({
            message : 'Suppression du cours avec succèss !'
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
    store, showAll, showByName, update, destroy
}
    