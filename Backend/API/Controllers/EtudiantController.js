const Etudiant = require('../../Database/Etudiant');
const Image = require('../../Database/Image');
const Audio = require('../../Database/Audio');
const Classe = require('../../Database/Classe');

module.exports.getAllEtudiant = async (req, res) =>{
    const { page } = req.query;
    
    try {
        const LIMIT = 8;
        const startIndex = (Number(page) - 1) * LIMIT; // get the starting index of every page
    
        const total = await Etudiant.countDocuments({});
        const etudiantList = await Etudiant.find().sort({ _id: -1 }).limit(LIMIT).skip(startIndex);

        res.json({ data: etudiantList, currentPage: Number(page), numberOfPages: Math.ceil(total / LIMIT)});
    } catch (error) {    
        res.status(404).json({ message: error.message });
    }
};

module.exports.getEtudiantByStatus = async (req, res) => {
   

    try {
        const etudiants = await Etudiant.find({ statut:false });

        res.json({ data: etudiants });
    } catch (error) {    
        res.status(404).json({ message: error.message });
    }
};


module.exports.createEtudiant = async (req, res) => {
    //const etudiant = req.body;

    const etudiant = {
        matricule: req.body.matricule,
        nom: req.body.nom,
        empreinte: req.body.empreinte,
        statut: false
    }

    const imageEmpreinte = req.body.imageEmpreinte
    const voix = req.body.voix

    const audio = { nom: req.body.nom, audio: { data: voix, contentType: "audio/wav"}}

    const image = { nom: req.body.nom, img: { data: imageEmpreinte, contentType: "image"}}

    const classeEtudiant = await Classe.findOne({ nom: req.body.classe })

    if(!(classeEtudiant instanceof Classe)) return res.status(404).send(`No Class with name : ${req.body.classe}`);

    const postAudio = await Audio.create(audio);
    if(!(postAudio instanceof Audio)) return res.status(500).send(`An error occur`);

    const postImage = await Image.create(image);
    if(!(postImage instanceof Image)) return res.status(500).send(`An error occur`);
    const newEtudiant = new Etudiant({ ...etudiant,classe:classeEtudiant._id ,voix:postAudio._id,imageEmpreinte:postImage._id })

    try {
        await newEtudiant.save();

        res.status(201).json(newEtudiant);
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};


module.exports.updateEtudiant = async (req, res) => {
    const { id } = req.params;
  
    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No Student with id: ${id}`);

    const updatedData = { statut:true };

    await PostMessage.findByIdAndUpdate(id, { $set: updatedData })
    .then(()=>{
        res.json({ message: "Enrollement validé"})
    })
    .catch(error => {
        res.status(409).json({ message: error.message })
    } )


};


module.exports.updateAllEtudiant = async (req, res) => {
        // create a filter to update all student with statut false
        const filter = { statut: false };
        // set statut to true
        const updateDoc = {
          $set: {
            statut: true,
          },
        };

        try {
            await Etudiant.updateMany(filter, updateDoc);
    
            res.json({ message: "Enrollements validés" });
    
        } catch (error) {
            res.status(409).json({ message: error.message });
        }
};


module.exports.deleteEtudiant  = async (req, res) => {
    const { id } = req.params;

    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No student with id: ${id}`);

    try {
        await Etudiant.findByIdAndRemove(id);

        res.json({ message: "Student deleted successfully." });

    } catch (error) {
        res.status(409).json({ message: error.message });
    }
};