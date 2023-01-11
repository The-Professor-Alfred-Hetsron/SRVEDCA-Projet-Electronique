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
            message:'Une erreur est survenue!', content: error.message
        })
    })
}



// Show the list of classes (Read)
const showAll =(req, res, next) => {
    Classe.find().sort({nom: 1})
    .then(response =>{
        res.json(response)
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!', content: error.message
        })
    })
}

// Show one class by name (Read)
const showOne = async (req, res, next) => {
    try {
        let classe = await Classe.findOne({nom: req.params.nom})
        res.json(classe)
    } catch (error) {
        res.json({
            message:'Une erreur est survenue!', content: error.message
        })
    }
}


// Update a class (Update)
const update = async (req, res, next) =>{
    try {
        let classe = await Classe.findOne({nom: req.body.nom})
        if(!classe) throw new Error('Classe non trouvée.')

        classe.nom = req.body.nom ? req.body.nom : classe.nom
        classe.salle = req.body.salle ? req.body.salle : classe.salle
        await classe.save()

        res.json({
            message: 'Classe modifiée avec sucèss!'
        })

    } catch (error) {
        res.json({
            message:'Une erreur est survenue!', content: error.message
        })
    }

}

// Delete a class (Delete)
const destroy = (req, res, next) =>{
    Classe.findOneAndRemove({nom: req.body.nom})
    .then(() => {
        res.json({
            message: 'Classe supprimeé avec sucèss!'
        })
    })
    .catch(error => {
        res.json({
            message:'Une erreur est survenue!', content: error.message
        })
    })
}


module.exports = {
    store, showAll, showOne, update, destroy
}