const Cours = require('../../Database/Cours')

// Creer un nouveau cours
const store = async (req, res, next) => {
     let cours = new Cours({
        code : req.body.code,
        nom :  req.body.nom,
        classe : req.body.classe,
        semestre : req.body.semestre
     })

    try {
        let result = await cours.save()
        res.status(200).json({
            message : 'Nouveau Cours ajouté !', content: await result.populate('classe')
       })
    } catch (error) {
        res.status(400).json({
            message : 'Une erreur est survenue !', content: error.message
        })
    }
}

//Montrer la liste de cours
const showAll=(req, res, next) => {
    Cours.find().sort({code: 1}).populate('classe')
    .then(response =>{
        res.status(200).json(response)
    })
    .catch(error =>{
        res.status(400).json({
            message:'Une erreur est survenue', content: error.message
        })
    })
}

//Afficher un cours par son nom

const showByName=(req, res, next) => {
    let coursName = req.params.nom
    Cours.findOne({nom : coursName}).populate('classe')
    .then(response =>{
        res.status(200).json(response)
    })
    .catch(error =>{
        res.status(400).json({
            message:'Une erreur est survenue', content: error.message
        })
    })
}

//Modifier un cours
const update = async (req, res, next) => {

    try {
        let cours = await Cours.findById(req.body.id)
        if(!cours) throw new Error('Cours non trouvé.')

        if(req.body.code) cours.code = req.body.code
        if(req.body.nom) cours.nom = req.body.nom
        if(req.body.classe) cours.classe = req.body.classe
        if(req.body.semestre) cours.semestre = req.body.semestre

        cours = await (await cours.save()).populate('classe')

        res.status(200).json({
            message: 'Cours modifiée avec sucèss!', content: cours
        })

    } catch (error) {
        res.status(400).json({
            message:'Une erreur est survenue!', content: error.message
        })
    }

}

// Supprimer un cours
const destroy = (req, res, next) => {
    Cours.findByIdAndRemove(req.params.id)
    .then(() => {
        res.status(200).json({
            message : 'Suppression du cours avec succèss !'
        })
    })
    .catch(error => {
        res.status(400).json({
            message : 'Une erreur est survenue !', content: error.message
        })
    })
}


// exportation des fonctions
module.exports = {
    store, showAll, showByName, update, destroy
}
    