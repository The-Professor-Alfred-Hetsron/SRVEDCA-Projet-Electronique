const Classe = require('../../Database/Classe')

// Create an new class (Create)
const store = (req, res, next) => {
    let classe = new Classe({
        nom: req.body.nom,
        salle: req.body.salle
    })
    classe.save()
    .then(response =>{
        res.json({
            message: 'Classe enregistrée avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}



// Show the list of classes (Read)
const showAll =(req, res, next) => {
    Classe.find()
    .then(response =>{
        res.json({
            response
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}


// Update a class (Update)
const update = (req, res, next) =>{
    let classeNom = req.body.nom

    let updateData = {
        nom: req.body.nom,
        salle: req.body.salle
    }

    Classe.findOneAndUpdate({nom:classeNom}, {$set: updateData})
    .then(() => {
        res.json({
            message: 'Classe modifiée avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}

// Delete a class (Delete)
const destroy = (req, res, next) =>{
    let classeNom = req.params.classeNom
    Classe.findOneAndRemove({nom:classeNom})
    .then(() => {
        res.json({
            message: 'Classe supprimeé avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!'
        })
    })
}


module.exports = {
    store, showAll, update, destroy
}